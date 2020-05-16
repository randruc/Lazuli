/**
 * @file src/kern/scheduler.c
 * @brief Lazuli scheduler implementation.
 * @date Mar 2018
 * @author Remi Andruccioli
 *
 * This file describes the implementation of the Lazuli scheduler.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>
#include <Lazuli/lazuli.h>

#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/clock_24.h>
#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler.h>

/**
 * A pointer to the current running task.
 */
Task *currentTask;

/**
 * The queue of ready tasks.
 */
static Lz_LinkedList readyTasks = LINKED_LIST_INIT;

/**
 * The queue of tasks waiting activation.
 *
 * i.e. Tasks that have come to completion for their period.
 *
 * Next status for these tasks is: READY.
 */
static Lz_LinkedList waitingTasks = LINKED_LIST_INIT;

/**
 * The queue of terminated tasks.
 */
static Lz_LinkedList terminatedTasks = LINKED_LIST_INIT;

/**
 * The queue of aborted tasks.
 */
static Lz_LinkedList abortedTasks = LINKED_LIST_INIT;

/**
 * The idle task.
 *
 * This task is run when no other task is ready to run.
 */
static NOINIT Task *idleTask;

/**
 * Contains default values for Lz_TaskConfiguration.
 */
static PROGMEM const Lz_TaskConfiguration DefaultTaskConfiguration = {
  NULL                                             /**< member: name       */,
  LZ_CONFIG_DEFAULT_TASK_STACK_SIZE                /**< member: stackSize  */,
  0                                                /**< member: period     */,
  0                                                /**< member: completion */
};

/**
 * The scheduler idle task.
 *
 * This task is executed when no other task is ready for execution.
 */
static void
IdleTask(void)
{
  for (;;) {
    if (LZ_CONFIG_ON_IDLE_SLEEP) {
      Arch_CpuSleep();
    }
  }
}

/**
 * Prepare the first context of the task so it will be ready when switching
 * context for the first time (i.e. run the scheduler).
 *
 * @param task A pointer to the Task to prepare.
 */
static void
PrepareTaskContext(Task * const task)
{
  TaskContextLayout * const contextLayout
    = (TaskContextLayout *)(ALLOW_ARITHM(task->stackPointer)
                            - sizeof(TaskContextLayout) + 1);

  contextLayout->pc = ReverseBytesOfFunctionPointer(task->entryPoint);
  contextLayout->terminationCallback =
    ReverseBytesOfFunctionPointer(Lz_Task_Terminate);

  task->stackPointer = ALLOW_ARITHM((void*)contextLayout) - 1;
}

/**
 * Insert a task in a list, keeping priorities ordered by period (shortest to
 * largest).
 *
 * If tasks with the same priority (period) than the @p taskToInsert already
 * exist in the list, the @p taskToInsert will be inserted after existing tasks
 * of the same priority.
 *
 * @param list The list in which to insert the task.
 * @param taskToInsert The task to insert in the list.
 */
static void
InsertTaskByPeriodPriority(Lz_LinkedList * const list,
                           Task * const taskToInsert)
{
  Task *task;

  if (NULL == list || NULL == taskToInsert) {
    return;
  }

  List_ForEach (list, Task, task, stateQueue) {
    if (task->period > taskToInsert->period) {
      List_InsertBefore(list,
                        &task->stateQueue,
                        &taskToInsert->stateQueue);

      return;
    }
  }

  List_Append(list, &taskToInsert->stateQueue);
}

/**
 * Elect the new current task.
 *
 * The election is done by setting the currentTask pointer to the elected task.
 *
 * This function is called at each clock tick (triggered by the timer at the
 * rate of the system time resolution).
 *
 * This function updates all tasks lists accordingly to the different realtime
 * parameters and status of each task.
 */
static void
Schedule(void)
{
  Task *loopTask;
  Lz_LinkedListElement *linkedListElement;

  if (currentTask != idleTask) {
    currentTask->timeUntilCompletion--;

    if (WAIT_ACTIVATION == currentTask->taskToSchedulerMessage ||
        0 == currentTask->timeUntilCompletion) {
      List_Append(&waitingTasks, &currentTask->stateQueue);
    } else {
      InsertTaskByPeriodPriority(&readyTasks, currentTask);
    }
  }

  currentTask->taskToSchedulerMessage = NO_MESSAGE;

  List_ForEach(&readyTasks, Task, loopTask, stateQueue) {
    loopTask->timeUntilActivation--;

    if (0 == loopTask->timeUntilActivation) {
      if (loopTask->timeUntilCompletion > 0) {
        /* TODO: Missed deadline */
      } else {
        loopTask->timeUntilActivation = loopTask->period;
      }
    }
  }

  List_RemovableForEach(&waitingTasks,
                        Task,
                        loopTask,
                        stateQueue,
                        linkedListElement) {
    loopTask->timeUntilActivation--;

    if (0 == loopTask->timeUntilActivation) {
      linkedListElement = List_Remove(&waitingTasks, &loopTask->stateQueue);
      InsertTaskByPeriodPriority(&readyTasks, loopTask);

      loopTask->timeUntilActivation = loopTask->period;
      loopTask->timeUntilCompletion = loopTask->completion;
    }
  }

  linkedListElement = List_PickFirst(&readyTasks);
  if (NULL != linkedListElement) {
    currentTask = CONTAINER_OF(linkedListElement, stateQueue, Task);
  } else {
    currentTask = idleTask;
  }
}

/**
 * Callback of SchedulerOperations.registerTask() for registering a user task.
 *
 * @param taskConfiguration A pointer to an Lz_TaskConfiguration containing
 *                          the configuration of the task being registered.
 *                          This parameter can never be _NULL_, but some of
 *                          its fields can contain default configuration
 *                          values.
 *
 * @return A pointer to the newly allocated and initialized Task.
 */
static Task *
CallbackRegisterUserTask(const Lz_TaskConfiguration * const taskConfiguration)
{
  Task *newTask;

  if (0 == taskConfiguration->period || 0 == taskConfiguration->completion) {
    return NULL;
  }

  newTask = KIncrementalMalloc(sizeof(Task));
  if (NULL == newTask) {
    return NULL;
  }

  newTask->period = taskConfiguration->period;
  newTask->completion = taskConfiguration->completion;

  newTask->timeUntilActivation = newTask->period;
  newTask->timeUntilCompletion = newTask->completion;

  newTask->taskToSchedulerMessage = NO_MESSAGE;

  List_InitLinkedListElement(&newTask->stateQueue);
  InsertTaskByPeriodPriority(&readyTasks, newTask);

  return newTask;
}

/**
 * Callback of SchedulerOperations.registerTask() for registering the scheduler
 * idle task.
 *
 * @return A pointer to the allocated and initialized idle Task.
 */
static Task *
CallbackRegisterIdleTask(void)
{
  idleTask = KIncrementalMalloc(sizeof(Task));
  if (NULL == idleTask) {
    return NULL;
  }

  idleTask->taskToSchedulerMessage = NO_MESSAGE;

  return idleTask;
}

/**
 * Register a new task.
 *
 * @param taskEntryPoint The entry point of the task to register.
 *                       i.e. A pointer to the function representing the task.
 * @param taskConfiguration A pointer to an Lz_TaskConfiguration containing the
 *                          configuration of the task being registered.
 *                          If NULL is passed, then default values are applied
 *                          for all parameters.
 * @param idleTask A boolean value indicating that the task to register is the
 *                 scheduler idle task.
 *
 * @return
 *         - _true_ if the task has been registered without error.
 *         - _false_ if an error occured during registration.
 */
static bool
RegisterTask(void (* const taskEntryPoint)(void),
             const Lz_TaskConfiguration * taskConfiguration,
             bool idleTask)
{
  Lz_TaskConfiguration defaultConfiguration;
  Task *newTask;
  void *taskStack;
  size_t desiredStackSize;

  if (NULL == taskConfiguration) {
    Arch_LoadFromProgmem(&DefaultTaskConfiguration,
                         &defaultConfiguration,
                         sizeof(Lz_TaskConfiguration));
    taskConfiguration = &defaultConfiguration;
  }

  if (idleTask) {
    newTask = CallbackRegisterIdleTask();
  } else {
    newTask = CallbackRegisterUserTask(taskConfiguration);
  }

  if (NULL == newTask) {
    return false;
  }

  desiredStackSize = taskConfiguration->stackSize
    /* We add enough space to contain the context of a task on the stack */
    + sizeof(TaskContextLayout)
    /* Plus 1 call to save_context_on_stack (in startup.S) */
    + sizeof(void *);

  taskStack = KIncrementalMalloc(desiredStackSize);
  if (NULL == taskStack) {
    return false;
  }

  newTask->name = taskConfiguration->name;
  newTask->entryPoint = taskEntryPoint;
  newTask->stackSize = desiredStackSize;
  newTask->stackOrigin = ALLOW_ARITHM(taskStack) + desiredStackSize - 1;
  newTask->stackPointer = ALLOW_ARITHM(taskStack) + desiredStackSize - 1;

  PrepareTaskContext(newTask);

  return true;
}

/**
 * Register the idle task.
 *
 * @return
 *         - _true_ if the task has been registered without error.
 *         - _false_ if an error occured during registration.
 */
static bool
RegisterIdleTask(void)
{
  Lz_TaskConfiguration taskConfiguration;

  Lz_TaskConfiguration_Init(&taskConfiguration);

  taskConfiguration.stackSize = LZ_CONFIG_IDLE_TASK_STACK_SIZE;

  if (LZ_CONFIG_IDLE_TASK_HAS_NAME) {
    taskConfiguration.name = LZ_CONFIG_IDLE_TASK_NAME;
  }

  return RegisterTask(IdleTask, &taskConfiguration, true);
}

/**
 * @name Kernel API
 * @{
 */

/*
 * We could declare this function as static.
 * But if we do this we can't unit test it...
 */
void
(*ReverseBytesOfFunctionPointer(void (* const pointer)(void)))(void)
{
  const uint8_t maxIndex = sizeof(pointer) - 1;
  const uint8_t * const oldPointerPointer = (const uint8_t * const)&pointer;
  void (*newPointer)(void);
  uint8_t * const newPointerPointer = (uint8_t * const)&newPointer;
  uint8_t i;

  for (i = 0; i <= maxIndex; ++i) {
    newPointerPointer[maxIndex - i] = oldPointerPointer[i];
  }

  return newPointer;
}

void
Scheduler_Init(void)
{
  Arch_InitSystemTimer();
}

void
Scheduler_ManageTaskTermination(void * const sp)
{
  if (LZ_CONFIG_SAVE_TASK_CONTEXT_ON_TERMINATION) {
    currentTask->stackPointer = sp;
  }

  List_Append(&terminatedTasks, &currentTask->stateQueue);

  Arch_EnableInterrupts();

  for(;;);

  /*
   * TODO: we have a problem here.
   * Here, CPU is now idle.
   * But Schedule() doesn't run the elected task.
   *
   * I think one good solution is to enter idle, and wait for the next clock
   * tick.
   */
}

void
Scheduler_AbortTask(void * const sp)
{
  currentTask->stackPointer = sp;

  List_Append(&abortedTasks, &currentTask->stateQueue);

  Arch_EnableInterrupts();

  for(;;);

  /*
   * TODO: we have a problem here.
   * Here, CPU is now idle.
   * But Schedule() doesn't run the elected task.
   *
   * I think one good solution is to enter idle, and wait for the next clock
   * tick.
   */
}

void
Scheduler_HandleInterrupt(void * const sp, const uint8_t interruptCode)
{
  if (LZ_CONFIG_CHECK_INTERRUPT_CODE_OVER_LAST_ENTRY) {
    if (interruptCode > INT_LAST_ENTRY) {
      Kernel_Panic();
    }
  }

  currentTask->stackPointer = sp;

  if (INT_TIMER1COMPA == interruptCode) {
    if (LZ_CONFIG_MODULE_CLOCK_24_USED) {
      Clock24_Increment();
    }

    Schedule();
  }

  Arch_RestoreContextAndReturnFromInterrupt(currentTask->stackPointer);
}

/* TODO: Maybe think about rename this one to WaitInterrupt */
void
Scheduler_WaitEvent(void * const sp, const uint8_t eventCode)
{
  /* TODO: Implement this */
  UNUSED(sp);
  UNUSED(eventCode);
}

void
Scheduler_WakeupTasksWaitingMutex(Lz_LinkedList * const waitingTasks)
{
  /* TODO: Implement this */
  UNUSED(waitingTasks);
}

void
Scheduler_WaitMutex(void * const sp, Lz_LinkedList * const waitingTasks)
{
  /* TODO: Implement this */
  UNUSED(sp);
  UNUSED(waitingTasks);
}

/** @} */

/** @name User API */
/** @{             */

void
Lz_TaskConfiguration_Init(Lz_TaskConfiguration * const taskConfiguration)
{
  if (NULL == taskConfiguration) {
    return;
  }

  Arch_LoadFromProgmem(&DefaultTaskConfiguration,
                       taskConfiguration,
                       sizeof(Lz_TaskConfiguration));
}

bool
Lz_RegisterTask(void (* const taskEntryPoint)(void),
                const Lz_TaskConfiguration * taskConfiguration)
{
  return RegisterTask(taskEntryPoint, taskConfiguration, false);
}

void
Lz_Run(void)
{
  const Lz_LinkedListElement * const firstElement = List_PickFirst(&readyTasks);

  if (!RegisterIdleTask()) {
    Kernel_Panic();
  }

  Arch_StartSystemTimer();

  if (NULL != firstElement) {
    currentTask = CONTAINER_OF(firstElement, stateQueue, Task);
  } else {
    currentTask = idleTask;
  }

  Arch_RestoreContextAndReturnFromInterrupt(currentTask->stackPointer);
}

const char *
Lz_Task_GetName(void)
{
  return currentTask->name;
}

void
Lz_Task_WaitActivation(void)
{
  currentTask->taskToSchedulerMessage = WAIT_ACTIVATION;

  Arch_CpuSleep();
}

/** @} */

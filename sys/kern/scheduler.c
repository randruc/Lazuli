/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Lazuli scheduler implementation.
 * @copyright 2018-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the implementation of the Lazuli scheduler.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/list.h>
#include <Lazuli/mutex.h>

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
static Task *currentTask;

/**
 * The queues of ready tasks for each scheduling policy.
 *
 * @attention Indexed from highest priority policy to lowest.
 */
static NOINIT Lz_LinkedList readyTasks[LZ_SCHEDULING_POLICY_MAX + 1];

/**
 * The queue of tasks waiting activation.
 *
 * i.e. Tasks that have come to completion for their period.
 *
 * Next status for these tasks is: READY.
 */
static Lz_LinkedList waitingActivationTasks = LINKED_LIST_INIT;

/**
 * The table of waiting queues for interrupts.
 *
 * This table contains one entry per interrupt type.
 * In each entry is the queue of tasks waiting for that particular interrupt.
 * This table is indexed by the codes defined in interrupts.h.
 */
static NOINIT Lz_LinkedList waitingInterruptsTasks[INT_TOTAL];

/**
 * The queue of tasks waiting for their software timer to reach expiration.
 */
static Lz_LinkedList waitingTimerTasks = LINKED_LIST_INIT;

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
  NULL                              /**< member: name             */,
  LZ_CONFIG_DEFAULT_TASK_STACK_SIZE /**< member: stackSize        */,
  PRIORITY_RT                       /**< member: schedulingPolicy */,
  0                                 /**< member: priority         */,
  0                                 /**< member: period           */,
  0                                 /**< member: completion       */
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
 * Compare the "period" property of 2 tasks.
 *
 * @param task1 A valid pointer to the first Task.
 * @param task2 A valid pointer to the second Task.
 *
 * @return
 *         - _true_ if @p task1 has a bigger period than @p task2.
 *         - _false_ if @p task1 has a lower period than @p task2.
 */
static bool
PeriodComparer(const Task * const task1, const Task * const task2)
{
  return task1->period > task2->period;
}

/**
 * Compare the "priority" property of 2 tasks.
 *
 * @param task1 A valid pointer to the first Task.
 * @param task2 A valid pointer to the second Task.
 *
 * @return
 *         - _true_ if @p task1 has a bigger priority than @p task2.
 *         - _false_ if @p task1 has a lower priority than @p task2.
 */
static bool
PriorityComparer(const Task * const task1, const Task * const task2)
{
  return task1->priority > task2->priority;
}

/**
 * Insert a task in a list, keeping priorities ordered. The priority is
 * determined using the function pointer @p compareByProperty.
 *
 * If tasks with the same priority than the @p taskToInsert already exist in the
 * list, the @p taskToInsert will be inserted after existing tasks of the same
 * priority.
 *
 * @param list The list in which to insert the task.
 * @param taskToInsert The task to insert in the list.
 * @param compareByProperty A function pointer to the appropriate property
 *                          comparer.
 */
static void
InsertTaskByPriority(Lz_LinkedList * const list,
                     Task * const taskToInsert,
                     bool (*compareByProperty)(const Task * const ,
                                               const Task * const))
{
  Task *task;

  /*
   * This is commented on purpose.
   * TODO: Find a neat way to perform the equivalent. This function is static
   * so we master the parameters that are passed to that function.
   * Static checking could do the job.
   *
   * if (NULL == list || NULL == taskToInsert || NULL == compareByProperty) {
   *   return;
   * }
   */

  List_ForEach (list, Task, task, stateQueue) {
    if (compareByProperty(task, taskToInsert)) {
      List_InsertBefore(list,
                        &task->stateQueue,
                        &taskToInsert->stateQueue);

      return;
    }
  }

  List_Append(list, &taskToInsert->stateQueue);
}

/**
 * @cond false
 *
 * This assertion is necessary because in the function PickTaskToRun() we
 * iterate over all the possible declared values of lz_scheduling_policy_t to
 * index the array readyTasks.
 */
STATIC_ASSERT((LZ_SCHEDULING_POLICY_MAX + 1) == ELEMENTS_COUNT(readyTasks),
              The_readyTasks_array_size_is_wrong);
/** @endcond */

/**
 * Pick the task ready to run with the highest priority.
 *
 * To perform that operation, we iterate ready tasks in each policy, from the
 * highest priority to the lowest until we find a task that is ready to run.
 *
 * If no task is ready to run, we pick the idle task.
 *
 * @return A valid pointer to the next task to run.
 */
static Task*
PickTaskToRun(void)
{
  lz_scheduling_policy_t policy;

  for (policy = 0; policy < ELEMENTS_COUNT(readyTasks); ++policy) {
    const Lz_LinkedListElement * const linkedListElement
      = List_PickFirst(&readyTasks[policy]);

    if (NULL != linkedListElement) {
      return CONTAINER_OF(linkedListElement, stateQueue, Task);
    }
  }

  return idleTask;
}

/**
 * Update all registered cyclic RT tasks.
 *
 * This is to be done at every clock tick.
 */
static void
UpdateCyclicRealTimeTasks(void)
{
  Task *loopTask;
  Lz_LinkedListElement *iterator;

  /* Here we update the time until activation of all cyclic RT tasks */
  List_ForEach(&readyTasks[CYCLIC_RT], Task, loopTask, stateQueue) {
    --loopTask->timeUntilActivation;

    if (0 == loopTask->timeUntilActivation) {
      if (loopTask->timeUntilCompletion > 0) {
        /* TODO: Missed deadline */
      } else {
        loopTask->timeUntilActivation = loopTask->period;
      }
    }
  }

  /* After updating, we check if a cyclic RT task is ready to run */
  List_RemovableForEach(&waitingActivationTasks,
                        Task,
                        loopTask,
                        stateQueue,
                        iterator) {
    --loopTask->timeUntilActivation;

    if (0 == loopTask->timeUntilActivation) {
      iterator = List_Remove(&waitingActivationTasks, &loopTask->stateQueue);
      InsertTaskByPriority(&readyTasks[CYCLIC_RT], loopTask, PeriodComparer);

      loopTask->timeUntilActivation = loopTask->period;
      loopTask->timeUntilCompletion = loopTask->completion;
    }
  }
}

/**
 * Update all tasks waiting for the expiration of a software timer.
 *
 * This is to be done at every clock tick.
 */
static void
UpdateTasksWaitingSoftwareTimer(void)
{
  Task *task;
  Lz_LinkedListElement *iterator;

  List_RemovableForEach(&waitingTimerTasks, Task, task, stateQueue, iterator) {
    --task->timeUntilTimerExpiration;

    if (0 == task->timeUntilTimerExpiration) {
      iterator = List_Remove(&waitingTimerTasks, &task->stateQueue);
      InsertTaskByPriority(&readyTasks[PRIORITY_RT], task, PriorityComparer);
    }
  }
}

/**
 * Manage cyclic real-time tasks.
 *
 * @param message The message that the task passes to the scheduler.
 */
static void
ManageCyclicRealTimeTask(const lz_task_to_scheduler_message_t message)
{
  --currentTask->timeUntilCompletion;

  if (WAIT_ACTIVATION == message || 0 == currentTask->timeUntilCompletion) {
    List_Append(&waitingActivationTasks, &currentTask->stateQueue);

    return;
  }

  InsertTaskByPriority(&readyTasks[CYCLIC_RT], currentTask, PeriodComparer);
}

/**
 * Manage priority real-time tasks.
 *
 * @param message The message that the task passes to the scheduler.
 */
static void
ManagePriorityRealTimeTask(const lz_task_to_scheduler_message_t message)
{
  bool setCurrentTaskReady = false;

  if (WAIT_INTERRUPT == message) {
    const uint8_t interruptCode =
      *((uint8_t*)currentTask->taskToSchedulerMessageParameter);

    /*
     * If the requested interrupt code is not an acceptable value, we abort the
     * task.
     */
    if (LZ_CONFIG_CHECK_INTERRUPT_CODE_OVER_LAST_ENTRY &&
        (interruptCode > INT_LAST_ENTRY)) {
      List_Append(&abortedTasks, &currentTask->stateQueue);

      return;
    }

    List_Prepend(&waitingInterruptsTasks[interruptCode],
                 &currentTask->stateQueue);
  } else if (WAIT_SOFTWARE_TIMER == message) {
    currentTask->timeUntilTimerExpiration =
      *(lz_u_resolution_unit_t*)currentTask->taskToSchedulerMessageParameter;
    if (0 == currentTask->timeUntilTimerExpiration) {
      setCurrentTaskReady = true;
    } else {
      List_Append(&waitingTimerTasks, &currentTask->stateQueue);
    }
  } else if (LZ_CONFIG_MODULE_MUTEX_USED && (WAIT_MUTEX == message)) {
    Lz_Mutex * const mutex = currentTask->taskToSchedulerMessageParameter;
    List_Prepend(&mutex->waitingTasks, &currentTask->stateQueue);
  } else {
    setCurrentTaskReady = true;
  }

  if (setCurrentTaskReady) {
    InsertTaskByPriority(&readyTasks[PRIORITY_RT],
                         currentTask,
                         PriorityComparer);
  }
}

/**
 * Elect the new current task.
 *
 * The election is done by setting the currentTask pointer to the elected task.
 *
 * This function is called at each clock tick (triggered by the timer at the
 * rate of the system time resolution).
 *
 * This function updates all tasks lists accordingly to the different real-time
 * parameters and status of each task.
 */
static void
Schedule(void)
{
  /*
   * We call this function before adding new tasks waiting for a software timer
   * in order to avoid decrementing the expiration counter immediately after
   * adding the task to the corresponding waiting list.
   */
  UpdateTasksWaitingSoftwareTimer();

  if (currentTask != idleTask) {
    /*
     * currentTask->taskToSchedulerMessage is declared 'volatile'. However, we
     * are here in the scheduler, which is not interruptible. We can then cache
     * the message value in a variable to avoid reading it from memory each time
     * we access it.
     */
    const lz_task_to_scheduler_message_t message
      = currentTask->taskToSchedulerMessage;

    if (ABORT_TASK == message) {
      List_Append(&abortedTasks, &currentTask->stateQueue);
    } else if (TERMINATE_TASK == message) {
      List_Append(&terminatedTasks, &currentTask->stateQueue);
    } else {
      /* Array of function pointers, built on the stack */
      void
        (* const jumpToManager[LZ_SCHEDULING_POLICY_MAX + 1])
        (const lz_task_to_scheduler_message_t) =
        {
          ManageCyclicRealTimeTask,
          ManagePriorityRealTimeTask
        };

      jumpToManager[currentTask->schedulingPolicy](message);
    }
  }

  UpdateCyclicRealTimeTasks();

  currentTask->taskToSchedulerMessage = NO_MESSAGE;

  currentTask = PickTaskToRun();
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
  /*
   * Jump table to the appropriate comparer, depending of the desired
   * scheduling policy.
   */
  bool (* const comparers[LZ_SCHEDULING_POLICY_MAX + 1]) (const Task * const,
                                                          const Task * const) =
    {
      PeriodComparer,
      PriorityComparer
    };

  if (taskConfiguration->schedulingPolicy > LZ_SCHEDULING_POLICY_MAX) {
    return NULL;
  }

  if (CYCLIC_RT == taskConfiguration->schedulingPolicy &&
      (0 == taskConfiguration->period || 0 == taskConfiguration->completion)) {
    return NULL;
  }

  newTask = KIncrementalMalloc(sizeof(Task));
  if (NULL == newTask) {
    return NULL;
  }

  newTask->priority = taskConfiguration->priority;
  newTask->period = taskConfiguration->period;
  newTask->completion = taskConfiguration->completion;

  newTask->timeUntilActivation = newTask->period;
  newTask->timeUntilCompletion = newTask->completion;

  List_InitLinkedListElement(&newTask->stateQueue);

  InsertTaskByPriority(&readyTasks[taskConfiguration->schedulingPolicy],
                       newTask,
                       comparers[taskConfiguration->schedulingPolicy]);

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
 * @param isIdleTask A boolean value indicating that the task to register is the
 *                   scheduler idle task.
 *
 * @return
 *         - _true_ if the task has been registered without error.
 *         - _false_ if an error occurred during registration.
 */
static bool
RegisterTask(void (* const taskEntryPoint)(void),
             Lz_TaskConfiguration * taskConfiguration,
             const bool isIdleTask)
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
  } else if (taskConfiguration->stackSize < LZ_CONFIG_DEFAULT_TASK_STACK_SIZE) {
    taskConfiguration->stackSize = LZ_CONFIG_DEFAULT_TASK_STACK_SIZE;
  }

  if (isIdleTask) {
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
    + sizeof(void (*)(void));

  taskStack = KIncrementalMalloc(desiredStackSize);
  if (NULL == taskStack) {
    return false;
  }

  newTask->schedulingPolicy = taskConfiguration->schedulingPolicy;
  newTask->name = taskConfiguration->name;
  newTask->entryPoint = taskEntryPoint;
  newTask->stackSize = desiredStackSize;
  newTask->stackOrigin = ALLOW_ARITHM(taskStack) + desiredStackSize - 1;
  newTask->stackPointer = newTask->stackOrigin;
  newTask->timeUntilTimerExpiration = 0;
  newTask->taskToSchedulerMessage = NO_MESSAGE;

  PrepareTaskContext(newTask);

  return true;
}

/**
 * Register the idle task.
 *
 * @return
 *         - _true_ if the task has been registered without error.
 *         - _false_ if an error occurred during registration.
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

/**
 * @cond false
 *
 * Doxygen seems to not recognize this function declaration properly. An issue
 * has been declared to the project.
 * TODO: Keep an eye on it:
 * https://github.com/doxygen/doxygen/issues/7845
 *
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

/** @endcond */

void
Scheduler_Init(void)
{
  size_t i;
  const Lz_LinkedList linkedListInit = LINKED_LIST_INIT;

  Arch_InitSystemTimer();

  for (i = 0; i < ELEMENTS_COUNT(readyTasks); ++i) {
    Memory_Copy(&linkedListInit, &readyTasks[i], sizeof(linkedListInit));
  }

  for (i = 0; i < ELEMENTS_COUNT(waitingInterruptsTasks); ++i) {
    Memory_Copy(&linkedListInit,
                &waitingInterruptsTasks[i],
                sizeof(linkedListInit));
  }
}

void
Scheduler_AbortTask(void)
{
  currentTask->taskToSchedulerMessage = ABORT_TASK;

  Scheduler_SleepUntilEndOfTimeSlice();
}

/*
 * This function is executed on the current task's stack. So go easy with stack
 * usage.
 */
void
Scheduler_HandleInterrupt(const uint8_t interruptCode)
{
  Task *loopTask;
  Lz_LinkedListElement *iterator;

  if (LZ_CONFIG_CHECK_INTERRUPT_CODE_OVER_LAST_ENTRY) {
    if (interruptCode > INT_LAST_ENTRY) {
      Kernel_Panic();
    }
  }

  List_RemovableForEach(&waitingInterruptsTasks[interruptCode],
                        Task,
                        loopTask,
                        stateQueue,
                        iterator) {
    iterator = List_Remove(&waitingInterruptsTasks[interruptCode],
                           &loopTask->stateQueue);
    InsertTaskByPriority(&readyTasks[PRIORITY_RT], loopTask, PriorityComparer);
  }
}

void
Scheduler_HandleClockTick(void * const sp)
{
  currentTask->stackPointer = sp;

  if (LZ_CONFIG_MODULE_CLOCK_24_USED) {
    Clock24_Increment();
  }

  Schedule();

  Arch_RestoreContextAndReturnFromInterrupt(currentTask->stackPointer);
}

void
Scheduler_WakeupTasksWaitingMutex(Lz_Mutex * const mutex)
{
  Task *loopTask;
  Lz_LinkedListElement *iterator;

  /* TODO: Ugly */
  if (!LZ_CONFIG_MODULE_MUTEX_USED) {
    UNUSED(mutex);
    UNUSED(loopTask);
    UNUSED(iterator);

    return;
  }

  List_RemovableForEach(&mutex->waitingTasks,
                        Task,
                        loopTask,
                        stateQueue,
                        iterator) {
    iterator = List_Remove(&mutex->waitingTasks,
                           &loopTask->stateQueue);
    InsertTaskByPriority(&readyTasks[PRIORITY_RT], loopTask, PriorityComparer);
  }
}

Task*
Scheduler_GetCurrentTask(void)
{
  return currentTask;
}

void
Scheduler_SleepUntilEndOfTimeSlice(void)
{
  /*
   * We use this do-while loop because the task can be woken up by any interrupt
   * source. In the case of standard interrupts (i.e. not a clock tick)
   * the task needs to sleep again after the interruption has been managed.
   * In other words, currentTask->taskToSchedulerMessage will be equal to
   * NO_MESSAGE only if the task's time slice has finished.
   */
  do {
    Arch_CpuSleep();
  } while (NO_MESSAGE != currentTask->taskToSchedulerMessage);
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
                Lz_TaskConfiguration * taskConfiguration)
{
  return RegisterTask(taskEntryPoint, taskConfiguration, false);
}

void
Lz_Run(void)
{
  if (!RegisterIdleTask()) {
    Kernel_Panic();
  }

  currentTask = PickTaskToRun();

  Arch_StartSystemTimer();

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

  Scheduler_SleepUntilEndOfTimeSlice();
}

void
Lz_Task_WaitInterrupt(uint8_t interruptCode)
{
  /* TODO: Check if the calling task's scheduling policy is PRIORITY_RT */

  currentTask->taskToSchedulerMessageParameter = &interruptCode;
  currentTask->taskToSchedulerMessage = WAIT_INTERRUPT;

  Scheduler_SleepUntilEndOfTimeSlice();
}

void
Lz_WaitTimer(lz_u_resolution_unit_t units)
{
  /* TODO: Check if the calling task's scheduling policy is PRIORITY_RT */

  currentTask->taskToSchedulerMessageParameter = &units;
  currentTask->taskToSchedulerMessage = WAIT_SOFTWARE_TIMER;

  Scheduler_SleepUntilEndOfTimeSlice();
}

void
Lz_Task_Terminate(void)
{
  currentTask->taskToSchedulerMessage = TERMINATE_TASK;

  Scheduler_SleepUntilEndOfTimeSlice();
}

/** @} */

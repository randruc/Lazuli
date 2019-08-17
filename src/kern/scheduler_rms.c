/**
 * @file src/kern/scheduler_rms.c
 * @brief Rate Monotonic Scheduler implementation.
 * @date May 2019
 * @author Remi Andruccioli
 *
 * This file describes the implementation of the Rate Monotonic tasks scheduler.
 */

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/list.h>

#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/clock_24.h>
#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler.h>
#include <Lazuli/sys/scheduler_rms.h>
#include <Lazuli/sys/task.h>

/**
 * Shorthand to cast base currentTask in the right type.
 * This way we obtain a bidirectionnal binding to currentTask cast as an RmsTask
 * pointer.
 */
/*
 * "All problems in computer science can be solved by another level of
 * indirection" - David Wheeler
 */
static RmsTask ** const currentTaskAsRms = (RmsTask**)&currentTask;

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
static NOINIT RmsTask *idleTask;

/**
 * Idle tak.
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
                           RmsTask * const taskToInsert)
{
  RmsTask *task;

  if (NULL == list || NULL == taskToInsert) {
    return;
  }

  List_ForEach (list, RmsTask, task, stateQueue) {
    if (task->period > taskToInsert->period) {
      List_InsertBefore(list,
                        &task->stateQueue,
                        &taskToInsert->stateQueue);

      return;
    }
  }

  List_Append(list, &(taskToInsert->stateQueue));
}

/**
 * Elect the new current task.
 *
 * The election is done by setting the pointer currentTask (or its binding
 * *currentTaskAsRms) to the elected task.
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
  RmsTask *loopTask;
  Lz_LinkedListElement *linkedListElement;

  if (*currentTaskAsRms != idleTask) {
    (*currentTaskAsRms)->timeUntilCompletion--;

    if (WAIT_ACTIVATION == (*currentTaskAsRms)->taskToSchedulerMessage ||
        0 == (*currentTaskAsRms)->timeUntilCompletion) {
      List_Append(&waitingTasks, &(*currentTaskAsRms)->stateQueue);
    } else {
      InsertTaskByPeriodPriority(&readyTasks, *currentTaskAsRms);
    }
  }

  (*currentTaskAsRms)->taskToSchedulerMessage = NO_MESSAGE;

  List_ForEach(&readyTasks, RmsTask, loopTask, stateQueue) {
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
                        RmsTask,
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
    *currentTaskAsRms = CONTAINER_OF(linkedListElement, stateQueue, RmsTask);
  } else {
    *currentTaskAsRms = idleTask;
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
  RmsTask *newTask;

  if (0 == taskConfiguration->period || 0 == taskConfiguration->completion) {
    return NULL;
  }

  newTask = KIncrementalMalloc(sizeof(RmsTask));
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

  return &newTask->base;
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
  idleTask = KIncrementalMalloc(sizeof(RmsTask));
  if (NULL == idleTask) {
    return NULL;
  }

  idleTask->taskToSchedulerMessage = NO_MESSAGE;

  return &idleTask->base;
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

  taskConfiguration.stackSize = LZ_CONFIG_RMS_IDLE_TASK_STACK_SIZE;

  if (LZ_CONFIG_RMS_IDLE_TASK_HAS_NAME) {
    taskConfiguration.name = LZ_CONFIG_RMS_IDLE_TASK_NAME;
  }

  return BaseScheduler_RegisterTask(IdleTask, &taskConfiguration, true);
}

void
Lz_Task_WaitActivation(void)
{
  (*currentTaskAsRms)->taskToSchedulerMessage = WAIT_ACTIVATION;

  Arch_CpuSleep();
}

/**
 * @name scheduler_base implementation
 *
 * @{
 */

static void
Init(void)
{
  Arch_InitSystemTimer();
}

static Task *
RegisterTask(const Lz_TaskConfiguration * const taskConfiguration,
             const bool idleTask)
{
  if (idleTask) {
    return CallbackRegisterIdleTask();
  } else {
    return CallbackRegisterUserTask(taskConfiguration);
  }
}

static void
Run(void)
{
  const Lz_LinkedListElement * const firstElement = List_PickFirst(&readyTasks);

  if(!RegisterIdleTask()) {
    Kernel_Panic();
  }

  Arch_StartSystemTimer();

  if (NULL != firstElement) {
    *currentTaskAsRms = CONTAINER_OF(firstElement, stateQueue, RmsTask);
  } else {
    *currentTaskAsRms = idleTask;
  }

  Arch_RestoreContextAndReturnFromInterrupt(currentTask->stackPointer);
}

static void
HandleInterrupt(void * const sp, const uint8_t interruptCode)
{
  currentTask->stackPointer = sp;

  if (INT_TIMER1COMPA == interruptCode) {
    if (LZ_CONFIG_USE_CLOCK_24) {
      Clock24_Increment();
    }

    Schedule();
  }

  Arch_RestoreContextAndReturnFromInterrupt(currentTask->stackPointer);
}

static void
ManageTaskTermination(void * const sp) {
  if (LZ_CONFIG_SAVE_TASK_CONTEXT_ON_TERMINATION) {
    currentTask->stackPointer = sp;
  }

  List_Append(&terminatedTasks, &(*currentTaskAsRms)->stateQueue);

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

static void
AbortTask(void * const sp) {
  currentTask->stackPointer = sp;

  List_Append(&abortedTasks, &(*currentTaskAsRms)->stateQueue);

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

/**
 * Defines the operations of the Rate Monotonic Scheduler.
 */
const SchedulerOperations RMSSchedulerOperations = {
  Init,                  /**< member: init                    */
  RegisterTask,          /**< member: registerTask            */
  Run,                   /**< member: run                     */
  HandleInterrupt,       /**< member: handleInterrupt         */
  NULL,                  /**< member: waitEvent               */
  NULL,                  /**< member: wakeupTasksWaitingMutex */
  NULL,                  /**< member: waitMutex               */
  ManageTaskTermination, /**< member: manageTaskTermination   */
  AbortTask              /**< member: abortTask               */
};

/** @} */

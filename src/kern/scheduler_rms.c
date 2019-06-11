/**
 * @file src/kern/scheduler_rms.c
 * @brief Rate Monotonic Scheduler implementation.
 * @date May 2019
 * @author Remi Andruccioli
 *
 * This file describes the implementation of the Rate Monotonic tasks scheduler.
 */

#include <Lazuli/common.h>
#include <Lazuli/list.h>

#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/clock_24.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler_base.h>
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

/*
 * TODO: When running idle, find out what to do with currentTask. Should it be
 * set to NULL ? Because when we run idle, we re-enable interrupts, so the next
 * interrupt will save the context of... nothing!!!
 */

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
 * This function is called at each clock tick (triggered by the timer at the
 * rate of the system time resolution).
 */
static void
Schedule(void)
{
  RmsTask *loopTask;
  Lz_LinkedListElement *linkedListElement;

  (*currentTaskAsRms)->timeUntilCompletion--;

  if (0 == (*currentTaskAsRms)->timeUntilCompletion) {
    InsertTaskByPeriodPriority(&waitingTasks, *currentTaskAsRms);
  } else {
    InsertTaskByPeriodPriority(&readyTasks, *currentTaskAsRms);
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
  }
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
RegisterTask(const Lz_TaskConfiguration * const taskConfiguration)
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

  List_InitLinkedListElement(&newTask->stateQueue);
  InsertTaskByPeriodPriority(&readyTasks, newTask);

  return &newTask->base;
}

static void
Run(void)
{
  Lz_LinkedListElement *firstElement;

  Arch_StartSystemTimer();

  firstElement = List_PickFirst(&readyTasks);
  if (NULL != firstElement) {
    *currentTaskAsRms = CONTAINER_OF(firstElement, stateQueue, RmsTask);
    Arch_RestoreContextAndReturnFromInterrupt(currentTask->stackPointer);
  } else {
    /* No task is ready for execution */
    /* Nothing to do! Idle. */
  }
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

/**
 * @file src/kern/scheduler_rr.c
 * @brief Round-Robin scheduler implementation.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file describes the implementation of the Round-Robin tasks scheduler.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/list.h>
#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/arch/AVR/timer_counter_0.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler_base.h>
#include <Lazuli/sys/scheduler_rr.h>
#include <Lazuli/sys/task.h>

/**
 * The queue of ready tasks.
 */
static Lz_LinkedList readyTasks = LINKED_LIST_INIT;

/**
 * The queue of terminated tasks.
 */
static Lz_LinkedList terminatedTasks = LINKED_LIST_INIT;

/**
 * The queue of aborted tasks.
 */
static Lz_LinkedList abortedTasks = LINKED_LIST_INIT;

/**
 * The table of waiting queues for interrupts.
 *
 * This table contains one entry per interrupt type.
 * In each entry is the queue of tasks waiting for that particular interrupt.
 * This table is indexed by the codes defined in interrupts.h.
 */
static NOINIT Lz_LinkedList waitingInterruptTasks[INT_TOTAL];

/**
 * Initialize each entry of the waitingInterruptTasks table.
 */
static void
InitWaitingInterruptTasksTable(void)
{
  uint8_t i;
  const Lz_LinkedList initValue = LINKED_LIST_INIT;

  for (i = 0; i < INT_TOTAL; ++i) {
    Memory_Copy(&initValue,
                &waitingInterruptTasks[i],
                sizeof(initValue));
  }
}

/**
 * Perform a scheduling.
 */
static void
Schedule(void)
{
  Lz_LinkedListElement *firstElement = List_PickFirst(&readyTasks);
  if (NULL != firstElement) {
    currentTask = (Task *)CONTAINER_OF(firstElement, stateQueue, RrTask);
    Arch_RestoreContextAndReturnFromInterrupt(currentTask->stackPointer);
  } else {
    /* No task is ready for execution */
    /* Nothing to do! Idle. */
    /* TODO: manage idle time */
    for (;;);
  }
}

/**
 * Handle "compare match A" interrupts from timer 0.
 */
static void
Timer0CompareMatchAHandler(void)
{
  List_Append(&readyTasks, &(((RrTask *)currentTask)->stateQueue));
  Schedule();
}

/**
 * @name scheduler_base implementation
 *
 * @{
 */

static void
Init(void)
{
  TimerCounter0 * const timer0 = GetTimerCounter0();

  timer0->tccr0a = TCCR0A_WGM01;
  timer0->tccr0b = TCCR0B_CS02 | TCCR0B_CS00;
  timer0->ocr0a = (uint8_t)80;
  timer0->tcnt0 = (uint8_t)0;

  TimerCounter0InterruptsEnable(TIMSK0_OCIE0A);

  InitWaitingInterruptTasksTable();
}

static Task *
RegisterTask(const Lz_TaskConfiguration * const taskConfiguration)
{
  RrTask *newTask = KIncrementalMalloc(sizeof(RrTask));
  if (NULL == newTask) {
    return NULL;
  }

  UNUSED(taskConfiguration);

  List_InitLinkedListElement(&(newTask->stateQueue));
  List_Append(&readyTasks, &(newTask->stateQueue));

  return &newTask->base;
}

static void
Run(void)
{
  Lz_LinkedListElement *first = List_PickFirst(&readyTasks);
  if (NULL == first) {
    Kernel_Panic();
  }

  currentTask = (Task *)CONTAINER_OF(first, stateQueue, RrTask);

  Arch_StartRunning(currentTask->stackPointer,
                    OFFSET_OF(pc, TaskContextLayout));
}

/**
 * Entry point of the interrupt handler for this scheduler.
 *
 * @param sp The stack pointer of the task being executed before the interrupt.
 *           This is used to save the task's context address.
 * @param interruptCode The code of the interrupt that occured.
 */
static void
HandleInterrupt(void * const sp, const uint8_t interruptCode)
{
  currentTask->stackPointer = sp;

  if (INT_TIMER0COMPA == interruptCode) {
    Timer0CompareMatchAHandler();
  } else {
    List_AppendList(&readyTasks, &waitingInterruptTasks[interruptCode]);
    Arch_RestoreContextAndReturnFromInterrupt(currentTask->stackPointer);
  }
}

/**
 * Entry point of the wait handler for this scheduler.
 *
 * @param sp The stack pointer of the task calling the wait routine.
 * @param eventCode The code of the wait event.
 */
static void
WaitEvent(void * const sp, const uint8_t eventCode)
{
  currentTask->stackPointer = sp;

  List_Append(&waitingInterruptTasks[eventCode],
              &(((RrTask *)currentTask)->stateQueue));

  Schedule();
}

/**
 * This function is to be called after a mutex has been unlocked.
 * Switch all tasks waiting for the mutex to be ready to run again.
 *
 * @param waitingTasks A pointer to the Lz_LinkedList of tasks waiting for the
 * mutex.
 */
static void
WakeupTasksWaitingMutex(Lz_LinkedList * const waitingTasks)
{
  if (NULL == waitingTasks) {
    return;
  }

  List_AppendList(&readyTasks, waitingTasks);
}

/**
 * This function is to be called if a mutex couldn't be acquired.
 * Place the current running tasks in the lists of tasks waiting for the mutex,
 * and schedule to run another task.
 *
 * @param sp The stack pointer of the task calling the wait routine.
 * @param waitingTasks A pointer to the Lz_LinkedList of tasks waiting for the
 * mutex.
 */
static void
WaitMutex(void * const sp, Lz_LinkedList * const waitingTasks)
{
  /* TODO: See if it's useful to test NULL == waitingTasks here... */
  currentTask->stackPointer = sp;

  List_Append(waitingTasks, &(((RrTask *)currentTask)->stateQueue));

  Schedule();
}

static void
ManageTaskTermination(void * const sp)
{
  if (LZ_CONFIG_SAVE_TASK_CONTEXT_ON_TERMINATION) {
    currentTask->stackPointer = sp;
  }

  List_Append(&terminatedTasks, &(((RrTask *)currentTask)->stateQueue));

  Schedule();
}

static void
AbortTask(void * const sp)
{
  currentTask->stackPointer = sp;

  List_Append(&abortedTasks, &(((RrTask *)currentTask)->stateQueue));

  Schedule();
}

/**
 * Defines the operations of the Round-Robin scheduler.
 */
const SchedulerOperations RRSchedulerOperations = {
  /** member: init */
  Init,

  /** member: registerTask */
  RegisterTask,

  /** member: run */
  Run,

  /** member: handleInterrupt */
  HandleInterrupt,

  /** member: waitEvent */
  WaitEvent,

  /** member: wakeupTasksWaitingMutex */
  LZ_CONFIG_USE_MUTEX ? WakeupTasksWaitingMutex : NULL,

  /** member: waitMutex */
  LZ_CONFIG_USE_MUTEX ? WaitMutex : NULL,

  /** member: manageTaskTermination */
  ManageTaskTermination,

  /** member: abortTask */
  AbortTask
};

/** @} */

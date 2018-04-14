/**
 * @file src/kern/scheduler_rr.c
 * @brief Round-Robin scheduler implementation.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file describes the implementation of the Round-Robin tasks scheduler.
 */

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

#include <Lazuli/sys/scheduler_base.h>
#include <Lazuli/sys/scheduler_rr.h>
#include <Lazuli/sys/task.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/arch/AVR/timer_counter_0.h>
#include <Lazuli/sys/arch/AVR/interrupts.h>

/**
 * The queue of ready tasks.
 */
static LinkedList readyQueue = LINKED_LIST_INIT;

/**
 * The queue of tasks waiting for interrupt INT0.
 */
static LinkedList waitingInt0Queue = LINKED_LIST_INIT;

/**
 * Perform a scheduling.
 */
static void
Schedule()
{
  LinkedListElement *firstElement = List_PickFirst(&readyQueue);
  if (NULL != firstElement) {
    currentTask = (Task *)CONTAINER_OF(firstElement, stateQueue, RrTask);
    restore_context_from_stack_and_reti(currentTask->stackPointer);
  } else {
    /* Nothing to do! Idle. */
  }
}

/** @name Interrupt handling */
/** @{                       */

/**
 * Handle "compare match A" interrupts from timer 0.
 */
static void
Timer0CompareMatchAHandler()
{
  List_Append(&readyQueue, &(((RrTask *)currentTask)->stateQueue));
  Schedule();
}

/**
 * Handle "External Interrupt Request 0".
 */
static void
Int0Handler()
{
  List_AppendList(&readyQueue, &waitingInt0Queue);
  restore_context_from_stack_and_reti(currentTask->stackPointer);
}

/**
 * Jump table to interrupt handlers.
 *
 * This jump table MUST be ordered by interrupt code values.
 */
static void (* const JumpToInterruptHandler[])() = {
  Panic,                      /**< index: INT_RESET       */
  Int0Handler,                /**< index: INT_INT0        */
  Panic,                      /**< index: INT_INT1        */
  Panic,                      /**< index: INT_PCINT0      */
  Panic,                      /**< index: INT_PCINT1      */
  Panic,                      /**< index: INT_PCINT2      */
  Panic,                      /**< index: INT_WDT         */
  Panic,                      /**< index: INT_TIMER2COMPA */
  Panic,                      /**< index: INT_TIMER2COMPB */
  Panic,                      /**< index: INT_TIMER2OVF   */
  Panic,                      /**< index: INT_TIMER1CAPT  */
  Panic,                      /**< index: INT_TIMER1COMPA */
  Panic,                      /**< index: INT_TIMER1COMPB */
  Panic,                      /**< index: INT_TIMER1OVF   */
  Timer0CompareMatchAHandler, /**< index: INT_TIMER0COMPA */
  Panic,                      /**< index: INT_TIMER0COMPB */
  Panic,                      /**< index: INT_SPISTC      */
  Panic,                      /**< index: INT_USARTRX     */
  Panic,                      /**< index: INT_USARTUDRE   */
  Panic,                      /**< index: INT_USARTTX     */
  Panic,                      /**< index: INT_ADC         */
  Panic,                      /**< index: INT_EEREADY     */
  Panic,                      /**< index: INT_ANALOGCOMP  */
  Panic,                      /**< index: INT_TWI         */
  Panic                       /**< index: INT_SPMREADY    */
};

STATIC_ASSERT(ELEMENTS_COUNT(JumpToInterruptHandler) == (INT_SPMREADY + 1),
              The_handlers_jump_table_MUST_reference_all_possible_interrupts);

/** @} */

/** @name scheduler_base implementation */
/** @{                                  */

static void
Init()
{
  TimerCounter0 * const timer0 = GetTimerCounter0();

  timer0->tccr0a = TCCR0A_WGM01;
  timer0->tccr0b = TCCR0B_CS02 | TCCR0B_CS00;
  timer0->ocr0a = (u8)80;
  timer0->tcnt0 = (u8)0;

  TimerCounter0InterruptsEnable(TIMSK0_OCIE0A);
}

static void
RegisterTask(void (* const taskEntryPoint)(),
             Lz_TaskConfiguration * const taskConfiguration)
{
  RrTask *newTask;
  void *taskStack;
  size_t desiredStackSize;
  const Lz_TaskConfiguration *configuration;

  if (NULL != taskConfiguration) {
    configuration = taskConfiguration;
  } else {
    configuration = &DefaultTaskConfiguration;
  }

  newTask = KIncrementalMalloc(sizeof(RrTask));
  if (NULL == newTask) {
    Panic();
  }

  desiredStackSize = configuration->stackSize
    /* We add enough space to contain the context of a task on the stack */
    + sizeof(TaskContextLayout)
    /* Plus 1 call to save_context_on_stack (in startup.S) */
    + sizeof(void*);

  taskStack = KIncrementalMalloc(desiredStackSize);
  if (NULL == taskStack) {
    Panic();
  }

  newTask->base.name = configuration->name;
  newTask->base.entryPoint = taskEntryPoint;
  newTask->base.stackPointer = ALLOW_ARITHM(taskStack) + desiredStackSize - 1;
  newTask->stateQueue.next = NULL;

  BaseSchedulerPrepareTaskContext((Task *)newTask);

  List_Append(&readyQueue, &(newTask->stateQueue));
}

static void
Run()
{
  LinkedListElement *first = List_PickFirst(&readyQueue);

  if (NULL == first) {
    Panic();
  }

  currentTask = (Task *)CONTAINER_OF(first, stateQueue, RrTask);

  start_running(currentTask->stackPointer, OFFSET_OF(pc, TaskContextLayout));
}

/**
 * Entry point of the interrupt handler for this scheduler.
 *
 * @param sp The stack pointer of the task being executed before the interrupt.
 *           This is used to save the task's context address.
 * @param interruptCode The code of the interrupt that occured.
 */
static void
HandleInterrupt(void * const sp, const u8 interruptCode)
{
  currentTask->stackPointer = sp;

  if (interruptCode < ELEMENTS_COUNT(JumpToInterruptHandler)) {
    JumpToInterruptHandler[interruptCode]();
  } else {
    Panic();
  }
}

/**
 * Entry point of the wait handler for this scheduler.
 *
 * @param sp The stack pointer of the task calling the wait routine.
 * @param eventCode The code of the wait event.
 */
static void
WaitEvent(void * const sp, const u8 eventCode)
{
  /* TODO: Manage all possible wait operations... */
  currentTask->stackPointer = sp;

  UNUSED(eventCode);

  List_Append(&waitingInt0Queue, &(((RrTask *)currentTask)->stateQueue));
  Schedule();
}

/**
 * Defines the operations of the Round-Robin scheduler.
 */
const SchedulerOperations RRSchedulerOperations = {
  Init,            /**< member: init                */
  RegisterTask,    /**< member: registerTask        */
  Run,             /**< member: run                 */
  HandleInterrupt, /**< member: handleInterrupt     */
  WaitEvent        /**< member: waitEvent           */
};

/** @} */

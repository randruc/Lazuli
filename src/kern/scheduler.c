/**
 * @file src/kern/scheduler.c
 * @brief Tasks scheduler.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file describes the implementation of the tasks scheduler.
 */

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

#include <Lazuli/sys/scheduler.h>
#include <Lazuli/sys/task.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/linker.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/arch/AVR/timer_counter_0.h>
#include <Lazuli/sys/arch/AVR/interrupts.h>

/**
 * Keeps a pointer to the current running task.
 */
static Task *currentTask;

/**
 * The queue of ready tasks.
 */
static LinkedList readyQueue = LINKED_LIST_INIT;

/**
 * The queue of tasks waiting for interrupt INT0.
 */
static LinkedList waitingInt0Queue = LINKED_LIST_INIT;

/* TODO: Maybe think about storing default task configuration in progmem */
/**
 * Contains default values for TaskConfiguration.
 */
static const Lz_TaskConfiguration defaultTaskConfiguration = {
  NULL                    /**< member: name      */,
  DEFAULT_TASK_STACK_SIZE /**< member: stackSize */,
  (u16)0                  /**< member: T         */,
  (u16)0                  /**< member: C         */,
  (u16)0                  /**< member: D         */
};

void
Scheduler_Init()
{
  TimerCounter0 * const timer0 = GetTimerCounter0();

  timer0->tccr0a = TCCR0A_WGM01;
  timer0->tccr0b = TCCR0B_CS02 | TCCR0B_CS00;
  timer0->ocr0a = (u8)80;
  timer0->tcnt0 = (u8)0;

  TimerCounter0InterruptsEnable(TIMSK0_OCIE0A);
}

/**
 * Perform a scheduling.
 */
static void
Schedule()
{
  LinkedListElement *firstElement = List_PickFirst(&readyQueue);
  if (NULL != firstElement) {
    currentTask = CONTAINER_OF(firstElement, stateQueue, Task);

    restore_context_from_stack_and_reti(currentTask->stackPointer);
  } else {
    /* Nothing to do! Idle. */
  }
}

/**
 * Handle "compare match A" interrupts from timer 0.
 */
void
Timer0CompareMatchAHandler()
{
  List_Append(&readyQueue, &(currentTask->stateQueue));
  Schedule();
}

/**
 * Set the current running task to wait for interrupt "INT0".
 *
 * This is done by putting the current task on the appropriate wainting queue.
 * This function is called from the ASM routine Lz_WaitInt0 after saving the
 * context.
 */
void
CurrentTaskWaitInt0()
{
  currentTask->stackPointer = (void *)SP;
  SP = &_ramend;

  List_Append(&waitingInt0Queue, &(currentTask->stateQueue));
  Schedule();
}

/**
 * Handle "External Interrupt Request 0".
 */
void
Int0Handler()
{
  List_AppendList(&readyQueue, &waitingInt0Queue);

  restore_context_from_stack_and_reti(currentTask->stackPointer);
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

  contextLayout->pc = (FuncVoidVoid)swap16((u16)task->entryPoint);
  task->stackPointer = ALLOW_ARITHM((void*)contextLayout) - 1;
}

void
Lz_RegisterTask(void (*taskEntryPoint)(),
                Lz_TaskConfiguration * const taskConfiguration)
{
  Task *newTask;
  void *taskStack;
  size_t desiredStackSize;
  const Lz_TaskConfiguration *configuration;

  if (NULL != taskConfiguration) {
    configuration = taskConfiguration;
  } else {
    configuration = &defaultTaskConfiguration;
  }

  newTask = KIncrementalMalloc(sizeof(Task));
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

  newTask->name = configuration->name;
  newTask->entryPoint = taskEntryPoint;
  newTask->stateQueue.next = NULL;
  newTask->stackPointer = ALLOW_ARITHM(taskStack) + desiredStackSize - 1;

  PrepareTaskContext(newTask);

  List_Append(&readyQueue, &(newTask->stateQueue));
}

void
Lz_InitTaskConfiguration(Lz_TaskConfiguration * const taskConfiguration)
{
  if (NULL == taskConfiguration) {
    return;
  }

  MemoryCopy(&defaultTaskConfiguration,
             taskConfiguration,
             sizeof(Lz_TaskConfiguration));
}

void
Lz_Run()
{
  LinkedListElement *first = List_PickFirst(&readyQueue);

  if (NULL == first) {
    Panic();
  }

  currentTask = CONTAINER_OF(first, stateQueue, Task);

  start_running(currentTask->stackPointer, OFFSET_OF(pc, TaskContextLayout));
}

/* TODO: Maybe move this somewhere else */
char const*
Lz_GetTaskName()
{
  return currentTask->name;
}

/**
 * Jump table to interrupt handlers.
 *
 * This jump table MUST be ordered by interrupt code values.
 */
static void (* const JumpToHandler[])() = {
  /**
   * INT_RESET: Reset, Entry point at boot.
   */
  Panic,

  /**
   * INT_INT0: External interrupt request 0.
   */
  Int0Handler,

  /**
   * INT_INT1: External interrupt request 1.
   */
  Panic,

  /**
   * INT_PCINT0: Pin change interrupt request 0.
   */
  Panic,

  /**
   * INT_PCINT1: Pin change interrupt request 1.
   */
  Panic,

  /**
   * INT_PCINT2: Pin change interrupt request 2.
   */
  Panic,

  /**
   * INT_WDT: Watchdog time-out interrupt.
   */
  Panic,

  /**
   * INT_TIMER2COMPA: Timer/counter2 compare match A.
   */
  Panic,

  /**
   * INT_TIMER2COMPB: Timer/counter2 compare match B.
   */
  Panic,

  /**
   * INT_TIMER2OVF: Timer/counter2 overflow.
   */
  Panic,

  /**
   * INT_TIMER1CAPT: Timer/counter1 capture event.
   */
  Panic,

  /**
   * INT_TIMER1COMPA: Timer/counter1 compare match A.
   */
  Panic,

  /**
   * INT_TIMER1COMPB: Timer/counter1 compare match B.
   */
  Panic,

  /**
   * INT_TIMER1OVF: Timer/counter1 overflow.
   */
  Panic,

  /**
   * INT_TIMER0COMPA: Timer/counter0 compare match A.
   */
  Timer0CompareMatchAHandler,

  /**
   * INT_TIMER0COMPB: Timer/counter0 compare match B.
   */
  Panic,

  /**
   * INT_SPISTC: SPI serial transfer complete.
   */
  Panic,

  /**
   * INT_USARTRX: USART Rx complete.
   */
  Panic,

  /**
   * INT_USARTUDRE: USART data register empty.
   */
  Panic,

  /**
   * INT_USARTTX: USART Tx complete.
   */
  Panic,

  /**
   * INT_ADC: ADC conversion complete.
   */
  Panic,

  /**
   * INT_EEREADY: EEPROM ready.
   */
  Panic,

  /**
   * INT_ANALOGCOMP: Analog comparator.
   */
  Panic,

  /**
   * INT_TWI: 2-wire serial interface.
   */
  Panic,

  /**
   * INT_SPMREADY: Store program memory ready.
   */
  Panic
};

STATIC_ASSERT(ELEMENTS_COUNT(JumpToHandler) == (INT_SPMREADY + 1),
              The_handlers_jump_table_MUST_reference_all_possible_interrupts);

/**
 * Entry point of the interrupt handler for this scheduler.
 *
 * @param sp The stack pointer of the task being executed before the interrupt.
 *           This is used to save the task's context address.
 * @param interruptCode The code of the interrupt that occured.
 */
void
HandleInterrupt(void * const sp, const u8 interruptCode)
{
  currentTask->stackPointer = sp;

  if (interruptCode < ELEMENTS_COUNT(JumpToHandler)) {
    JumpToHandler[interruptCode]();
  } else {
    Panic();
  }
}

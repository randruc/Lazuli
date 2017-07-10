/**
 * @file scheduler.c
 *
 * The file describes the implementation of the tasks scheduler.
 */

#include <Lazuli/common.h>
#include <Lazuli/scheduler.h>

#include <Lazuli/sys/scheduler.h>
#include <Lazuli/sys/task.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/linker.h>
#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/arch/AVR/timer_counter_0.h>
#include <Lazuli/sys/arch/AVR/arch.h>
#include <Lazuli/sys/arch/AVR/usart.h>

/**
 * Keeps a pointer to the current running task.
 */
static Task *currentTask;

/**
 * The queue of ready tasks.
 */
static LinkedListHead readyQueue = { NULL };

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
  LinkedListElement *firstElement;

  List_Append(&readyQueue, &(currentTask->stateQueue));

  firstElement = List_PickFirst(&readyQueue);
  if (NULL != firstElement) {
    currentTask = CONTAINER_OF(firstElement, stateQueue, Task);

    restore_context_from_stack_and_reti(currentTask->stackPointer);
  } else {
    /* Nothing to do! Idle. */
  }
}

/**
 * Handle "compare match A" interrupts from timer 0.
 *
 * This function is called by jumping, so the stack isn't touched.
 */
void
Timer0CompareMatchAHandler()
{
  /*
   * Important! No local variables in this function, so the compiler doesn't
   * move the stack pointer. We arrived here by a JUMP, not a CALL!
   * We can thus save the current task's stack.
   */
  currentTask->stackPointer = (void*)SP;
  SP = &_ramend;

  Schedule();
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
  TaskContextConfiguration * const contextConfiguration
    = (TaskContextConfiguration *)(ALLOW_ARITHM(task->stackPointer)
                                   - sizeof(TaskContextConfiguration) + 1);

  Usart_PutChar('p');
  Usart_NewLine();

  Usart_HexaPrint_Pointer(task);
  Usart_NewLine();

  contextConfiguration->pc = (VoidVoid)swap16((u16)task->entryPoint);
  task->stackPointer = contextConfiguration;

  Usart_HexaPrint_Pointer(contextConfiguration);
  Usart_NewLine();

  Usart_HexaPrint_Pointer((void*)(&(contextConfiguration->pc)));
  Usart_NewLine();

  Usart_HexaPrint_FunctionPointer(contextConfiguration->pc);
  Usart_NewLine();

  Usart_HexaPrint_Pointer(task->stackPointer);
  Usart_NewLine();
}

STATIC_ASSERT(DEFAULT_TASK_STACK_SIZE > sizeof(TaskContextConfiguration),
              "The task's stack must be big enough to hold its context.");

void
Lz_Scheduler_RegisterTask(void (*taskEntryPoint)())
{
  Task *newTask;
  void *taskStack;

  Usart_PutChar('r');
  Usart_NewLine();

  newTask = KIncrementalMalloc(sizeof(Task));
  if (NULL == newTask) {
    Panic();
  }

  Usart_HexaPrint_Pointer(newTask);
  Usart_NewLine();

  taskStack = KIncrementalMalloc(DEFAULT_TASK_STACK_SIZE);
  if (NULL == taskStack) {
    Panic();
  }

  Usart_HexaPrint_Pointer(taskStack);
  Usart_NewLine();

  newTask->entryPoint = taskEntryPoint;
  newTask->stateQueue.next = NULL;
  newTask->stackPointer = ALLOW_ARITHM(taskStack) + DEFAULT_TASK_STACK_SIZE - 1;

  Usart_HexaPrint_Pointer(newTask->stackPointer);
  Usart_NewLine();

  PrepareTaskContext(newTask);

  List_Append(&readyQueue, &(newTask->stateQueue));
}

void
Lz_Scheduler_Run()
{
  LinkedListElement *first = List_PickFirst(&readyQueue);

  Usart_PutChar('L');
  Usart_NewLine();

  if (NULL == first) {
    Panic();
  }

  currentTask = CONTAINER_OF(first, stateQueue, Task);

  Usart_HexaPrint_Pointer(currentTask);
  Usart_NewLine();

  Usart_HexaPrint_Pointer(currentTask->stackPointer);
  Usart_NewLine();

  Usart_HexaPrint_FunctionPointer(
    ((TaskContextConfiguration *)currentTask->stackPointer)->pc);
  Usart_NewLine();

  Usart_HexaPrint_Pointer(
        ((void*)&((TaskContextConfiguration *)currentTask->stackPointer)->pc));
  Usart_NewLine();


  start_running(currentTask->stackPointer);
}

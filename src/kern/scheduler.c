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
 * Handle "compare match A" interrupts from timer 0.
 */
void
Timer0CompareMatchAHandler()
{
  LinkedListElement *firstElement;
  Task *firstTask;

  currentTask->stackPointer = (void*)SP;
  SP = &_ramend;

  List_Append(&readyQueue, &(currentTask->stateQueue));

  firstElement = List_PickFirst(&readyQueue);
  if (NULL != firstElement) {
    firstTask = CONTAINER_OF(firstElement, stateQueue, Task);

    SP = firstTask->stackPointer;

    restore_context_from_stack_and_reti();
  } else {
    /* Nothing to do! Idle. */
  }
}

void
Lz_Scheduler_RegisterTask(void (*taskEntryPoint)())
{
  Task *newTask;
  void *taskStack;

  newTask = KIncrementalMalloc(sizeof(Task));
  if (NULL == newTask) {
    Panic();
  }

  taskStack = KIncrementalMalloc(DEFAULT_TASK_STACK_SIZE);
  if (NULL == taskStack) {
    Panic();
  }

  newTask->entryPoint = taskEntryPoint;
  newTask->stateQueue.next = NULL;
  newTask->stackPointer = ALLOW_ARITHM(taskStack) + DEFAULT_TASK_STACK_SIZE - 1;

  List_Append(&readyQueue, &(newTask->stateQueue));
}

void
Lz_Scheduler_Run()
{
  Task *task;
  LinkedListElement *first;

  if (RESET_KERNEL_STACK_ON_RUN) {
    SP = &_ramend;
  }

  first = List_PickFirst(&readyQueue);

  if (NULL == first) {
    Panic();
  }

  task = CONTAINER_OF(first, stateQueue, Task);
  currentTask = task;

  GlobalInterruptsEnable();
  task->entryPoint();
}

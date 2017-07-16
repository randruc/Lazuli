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
static LinkedList readyQueue = LINKED_LIST_INIT;

/**
 * The queue of tasks waiting for interrupt INT0.
 */
static LinkedList waitingInt0Queue = LINKED_LIST_INIT;

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
  currentTask->stackPointer = (void *)SP;
  SP = &_ramend;

  List_Append(&readyQueue, &(currentTask->stateQueue));
  Schedule();
}

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
 *
 * This function is called by jumping, so the stack isn't touched.
 */
void
Int0Handler()
{
  /*
   * Important! No local variables in this function, so the compiler doesn't
   * move the stack pointer. We arrived here by a JUMP, not a CALL!
   * We can thus save the current task's stack.
   */
  currentTask->stackPointer = (void *)SP;
  SP = &_ramend;

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
  TaskContextConfiguration * const contextConfiguration
    = (TaskContextConfiguration *)(ALLOW_ARITHM(task->stackPointer)
                                   - sizeof(TaskContextConfiguration) + 1);

  contextConfiguration->pc = (VoidVoid)swap16((u16)task->entryPoint);
  task->stackPointer = ALLOW_ARITHM((void*)contextConfiguration) - 1;
}

STATIC_ASSERT(DEFAULT_TASK_STACK_SIZE > sizeof(TaskContextConfiguration),
              "The task's stack must be big enough to hold its context.");

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

  PrepareTaskContext(newTask);

  List_Append(&readyQueue, &(newTask->stateQueue));
}

void
Lz_Scheduler_Run()
{
  LinkedListElement *first = List_PickFirst(&readyQueue);

  if (NULL == first) {
    Panic();
  }

  currentTask = CONTAINER_OF(first, stateQueue, Task);

  start_running(currentTask->stackPointer,
                OFFSET_OF(pc, TaskContextConfiguration));
}

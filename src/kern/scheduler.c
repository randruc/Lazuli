/**
 * @file src/kern/scheduler.c
 *
 * This file describes the implementation of the tasks scheduler.
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

/* TODO: Maybe think about storing default task configuration in progmem */
/**
 * Contains default values for TaskConfiguration.
 */
static const TaskConfiguration defaultTaskConfiguration = {
  DEFAULT_TASK_STACK_SIZE /**< member: stackSize */
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
  TaskContextLayout * const contextLayout
    = (TaskContextLayout *)(ALLOW_ARITHM(task->stackPointer)
                            - sizeof(TaskContextLayout) + 1);

  contextLayout->pc = (FuncVoidVoid)swap16((u16)task->entryPoint);
  task->stackPointer = ALLOW_ARITHM((void*)contextLayout) - 1;
}

void
Lz_Scheduler_RegisterTask(void (*taskEntryPoint)(),
                          TaskConfiguration * const taskConfiguration)
{
  Task *newTask;
  void *taskStack;
  size_t desiredStackSize;
  const TaskConfiguration *configuration;

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

  newTask->entryPoint = taskEntryPoint;
  newTask->stateQueue.next = NULL;
  newTask->stackPointer = ALLOW_ARITHM(taskStack) + desiredStackSize - 1;

  PrepareTaskContext(newTask);

  List_Append(&readyQueue, &(newTask->stateQueue));
}

void
Lz_Scheduler_InitTaskConfiguration(TaskConfiguration * const taskConfiguration)
{
  if (NULL == taskConfiguration) {
    return;
  }

  MemoryCopy(&defaultTaskConfiguration,
             taskConfiguration,
             sizeof(TaskConfiguration));
}

void
Lz_Scheduler_Run()
{
  LinkedListElement *first = List_PickFirst(&readyQueue);

  if (NULL == first) {
    Panic();
  }

  currentTask = CONTAINER_OF(first, stateQueue, Task);

  start_running(currentTask->stackPointer, OFFSET_OF(pc, TaskContextLayout));
}

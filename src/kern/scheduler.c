/**
 * @file scheduler.c
 *
 * The file describes the scheduler.
 */

#include <Lazuli/common.h>
#include <Lazuli/scheduler.h>
#include <Lazuli/sys/task.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/config.h>

/**
 * Keeps a pointer to the current running task.
 */
/* static Task *currentTask; */

/**
 * The queue of ready tasks.
 */
static LinkedListHead readyQueue = { NULL };

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
  LinkedListElement *first = List_PickFirst(&readyQueue);

  if(NULL == first) {
    return;
  }

  task = CONTAINER_OF(first, stateQueue, Task);

  task->entryPoint();
}

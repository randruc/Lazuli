/**
 * @file src/kern/scheduler_hpf.c
 * @brief Highest Priority First scheduler implementation.
 * @date Apr 2018
 * @author Remi Andruccioli
 *
 * This file describes the implementation of the Highest Priority First tasks
 * scheduler.
 */

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/list.h>
#include <Lazuli/sys/scheduler_base.h>
#include <Lazuli/sys/scheduler_hpf.h>

/**
 * The list of tasks ready to run, treated as a stack.
 * The tasks preempted by higher priority tasks, or ready to run after an
 * interrupt get pushed onto this stack.
 */
static LinkedList readyTasks = LINKED_LIST_INIT;

/* TODO: Comment that */
/* static */ void
InsertByPriority(HpfTask * const taskToInsert)
{
  HpfTask *task;

  if (NULL == taskToInsert) {
    return;
  }

  List_ForEach (&readyTasks, HpfTask, task, stateQueue) {
    if (task->priority < taskToInsert->priority) {
      List_InsertBefore(&readyTasks,
                        &(task->stateQueue),
                        &(taskToInsert->stateQueue));

      return;
    }
  }

  List_Append(&readyTasks, &(taskToInsert->stateQueue));
}

/** @name scheduler_base implementation */
/** @{                                  */

static void
Init()
{
}

static void
RegisterTask(void (* const taskEntryPoint)(),
             Lz_TaskConfiguration * const taskConfiguration)
{
  HpfTask *newTask;
  void *taskStack;
  size_t desiredStackSize;
  const Lz_TaskConfiguration *configuration;

  if (NULL == taskConfiguration) {
    configuration = &DefaultTaskConfiguration;
  } else {
    configuration = taskConfiguration;
  }

  newTask = KIncrementalMalloc(sizeof(HpfTask));
  if (NULL == newTask) {
    Panic();
  }

  desiredStackSize = configuration->stackSize
    /* We add enough space to contain the context of a task on the stack */
    + sizeof(TaskContextLayout)
    /* Plus 1 call to save_context_on_stack (in startup.S) */
    + sizeof(void *);

  taskStack = KIncrementalMalloc(desiredStackSize);
  if (NULL == taskStack) {
    Panic();
  }

  newTask->base.name = configuration->name;
  newTask->base.entryPoint = taskEntryPoint;
  newTask->base.stackPointer = ALLOW_ARITHM(taskStack) + desiredStackSize - 1;
  newTask->priority = configuration->priority;
  List_InitLinkedListElement(&(newTask->stateQueue));

  BaseScheduler_PrepareTaskContext((Task *)newTask);

  List_Push(&readyTasks, &(newTask->stateQueue));
}

static void
Run()
{
}

static void
HandleInterrupt(void * const sp, const u8 interruptCode)
{
  UNUSED(sp);
  UNUSED(interruptCode);
}

static void
WaitEvent(void * const sp, const u8 eventCode)
{
  UNUSED(sp);
  UNUSED(eventCode);
}

/**
 * Defines the operations of the Highest Priority First scheduler.
 */
const SchedulerOperations HPFSchedulerOperations = {
  Init,            /**< member: init                */
  RegisterTask,    /**< member: registerTask        */
  Run,             /**< member: run                 */
  HandleInterrupt, /**< member: handleInterrupt     */
  WaitEvent        /**< member: waitEvent           */
};

/** @} */

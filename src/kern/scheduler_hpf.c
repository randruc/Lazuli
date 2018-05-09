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

#include <Lazuli/sys/arch/arch.h>
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

/**
 * The task waiting for interrupt INT0.
 */
static HpfTask *waitingInt0Task = NULL;

/**
 * Insert a ready-to-run task in the list of ready tasks, keeping priorities
 * ordered.
 * This function is called if a task with a lower priority than the current
 * running task is ready to run (e.g. after waking-up by an interrupt).
 *
 * @param taskToInsert The task to insert in the list of ready-to-run tasks.
 */
static void
InsertTaskByPriority(HpfTask * const taskToInsert)
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

  InsertTaskByPriority(newTask);
}

static void
Run()
{
  LinkedListElement *first = List_PickFirst(&readyTasks);

  if (NULL == first) {
    Panic();
  }

  currentTask = (Task*)CONTAINER_OF(first, stateQueue, HpfTask);

  start_running(currentTask->stackPointer, OFFSET_OF(pc, TaskContextLayout));
}

static void
HandleInterrupt(void * const sp, const u8 interruptCode)
{
  if (NULL == waitingInt0Task) {
    restore_context_from_stack_and_reti(sp);
  }

  if (waitingInt0Task->priority <= ((HpfTask*)currentTask)->priority) {
    InsertTaskByPriority(waitingInt0Task);
    restore_context_from_stack_and_reti(sp);
  }

  currentTask->stackPointer = sp;

  List_Prepend(&readyTasks, &(((HpfTask*)currentTask)->stateQueue));

  currentTask = (Task*)waitingInt0Task;

  restore_context_from_stack_and_reti(currentTask->stackPointer);

  UNUSED(interruptCode);
}

static void
WaitEvent(void * const sp, const u8 eventCode)
{
  LinkedListElement *first;

  currentTask->stackPointer = sp;

  waitingInt0Task = (HpfTask*)currentTask;

  first = List_PickFirst(&readyTasks);
  if (NULL == first) {
    Panic();
  }

  currentTask = (Task*)CONTAINER_OF(first, stateQueue, HpfTask);

  restore_context_from_stack_and_reti(currentTask->stackPointer);

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

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
#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/list.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler_base.h>
#include <Lazuli/sys/scheduler_hpf.h>
#include <Lazuli/sys/task.h>

/**
 * The list of tasks ready to run, treated as a stack.
 * The tasks preempted by higher priority tasks, or ready to run after an
 * interrupt get pushed onto this stack.
 */
static LinkedList readyTasks = LINKED_LIST_INIT;

/**
 * The table of tasks waiting for interrupts.
 *
 * This table contains one entry per interrupt type.
 * In each entry is the queue of tasks waiting for that particular interrupt,
 * keep ordered by priority.
 * This table is indexed by the codes defined in interrupts.h.
 */
static LinkedList waitingInterruptTasks[INT_TOTAL];

/**
 * The idle task to be executed when no user task is ready for execution.
 *
 * This task has the lowest possible priority, so the user tasks are always
 * executed before the idle task if one is ready to run.
 */
static void
IdleTask()
{
  while (1);
}

/**
 * Initialize each entry of the waitingInterruptTasks table.
 */
static void
InitWaitingInterruptTasksTable()
{
  u8 i;
  const LinkedList initValue = LINKED_LIST_INIT;

  for (i = 0; i < INT_TOTAL; i++) {
    MemoryCopy(&initValue,
               &waitingInterruptTasks[i],
               sizeof(initValue));
  }
}

/**
 * Insert a task in a list, keeping priorities ordered.
 *
 * @param list The list in which to insert the task.
 * @param taskToInsert The task to insert in the list.
 */
static void
InsertTaskByPriority(LinkedList * const list, HpfTask * const taskToInsert)
{
  HpfTask *task;

  if (NULL == list || NULL == taskToInsert) {
    return;
  }

  List_ForEach (list, HpfTask, task, stateQueue) {
    if (task->priority < taskToInsert->priority) {
      List_InsertBefore(list,
                        &(task->stateQueue),
                        &(taskToInsert->stateQueue));

      return;
    }
  }

  List_Append(list, &(taskToInsert->stateQueue));
}

/** @name scheduler_base implementation */
/** @{                                  */

static void
Init()
{
  InitWaitingInterruptTasksTable();
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

  InsertTaskByPriority(&readyTasks, newTask);
}

static void
RegisterIdleTask()
{
  Lz_TaskConfiguration taskConfiguration;

  Lz_InitTaskConfiguration(&taskConfiguration);

  taskConfiguration.stackSize = 10;
  taskConfiguration.priority = -1;

  if (IDLE_TASK_HAS_NAME) {
    taskConfiguration.name = "idle";
  }

  RegisterTask(IdleTask, &taskConfiguration);
}

static void
Run()
{
  LinkedListElement *first;

  RegisterIdleTask();

  first = List_PickFirst(&readyTasks);
  if (NULL == first) {
    Panic();
  }

  currentTask = (Task*)CONTAINER_OF(first, stateQueue, HpfTask);

  start_running(currentTask->stackPointer, OFFSET_OF(pc, TaskContextLayout));
}

static void
HandleInterrupt(void * const sp, const u8 interruptCode)
{
  LinkedListElement *first;
  HpfTask *waitingTask;

  first = List_PickFirst(&waitingInterruptTasks[interruptCode]);

  if (NULL == first) {
    restore_context_and_return_from_interrupt(sp);
  }

  waitingTask = (HpfTask*)CONTAINER_OF(first, stateQueue, HpfTask);

  if (waitingTask->priority <= ((HpfTask*)currentTask)->priority) {
    InsertTaskByPriority(&readyTasks, waitingTask);
    restore_context_and_return_from_interrupt(sp);
  }

  currentTask->stackPointer = sp;

  List_Prepend(&readyTasks, &(((HpfTask*)currentTask)->stateQueue));

  currentTask = (Task*)waitingTask;

  restore_context_and_return_from_interrupt(currentTask->stackPointer);
}

static void
WaitEvent(void * const sp, const u8 eventCode)
{
  LinkedListElement *first;

  currentTask->stackPointer = sp;

  InsertTaskByPriority(&waitingInterruptTasks[eventCode],
                       (HpfTask*)currentTask);

  first = List_PickFirst(&readyTasks);
  if (NULL == first) {
    Panic();
  }

  currentTask = (Task*)CONTAINER_OF(first, stateQueue, HpfTask);

  restore_context_and_return_from_interrupt(currentTask->stackPointer);
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

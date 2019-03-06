/**
 * @file src/kern/scheduler_hpf.c
 * @brief Highest Priority First scheduler implementation.
 * @date Apr 2018
 * @author Remi Andruccioli
 *
 * This file describes the implementation of the Highest Priority First tasks
 * scheduler.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/list.h>
#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/assert.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/kernel.h>
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
  for (;;) {
    if (CONFIG_ON_IDLE_SLEEP) {
      Arch_CpuSleep();
    }
  }
}

/*
 * It would be really nice if waitingInterruptTasks could be declared to be
 * statically initialized to a specific value for every elements...
 * I know LINKED_LIST_INIT is full of zeros, but I don't really like to rely on
 * the BSS initialization to perform that. I would like something more
 * explicit.
 *
 * The problem here is that this waitingInterruptTasks array is placed in the
 * BSS section, so it is initialized to 0 on startup. As we perform the true
 * initialization here we thus perform 2 initializations, which isn't ideal...
 * One solution could be to declare this waitingInterruptTasks array without
 * initializaing it (like it is done here), but in a section that is not
 * initialized on startup, so not BSS. We could for example declare a special
 * section "runtime_initialized" that is excluded from the BSS startup
 * initialization.
 */
/**
 * Initialize each entry of the waitingInterruptTasks table.
 */
static void
InitWaitingInterruptTasksTable()
{
  uint8_t i;
  const LinkedList initValue = LINKED_LIST_INIT;

  for (i = 0; i < INT_TOTAL; ++i) {
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

static void
RegisterIdleTask()
{
  Lz_TaskConfiguration taskConfiguration;

  Lz_InitTaskConfiguration(&taskConfiguration);

  taskConfiguration.stackSize = CONFIG_HPF_IDLE_TASK_STACK_SIZE;
  taskConfiguration.priority = CONFIG_HPF_IDLE_TASK_PRIORITY;

  if (CONFIG_HPF_IDLE_TASK_HAS_NAME) {
    taskConfiguration.name = CONFIG_HPF_IDLE_TASK_NAME;
  }

  Lz_RegisterTask(IdleTask, &taskConfiguration);
}

/** @name scheduler_base implementation */
/** @{                                  */

static void
Init()
{
  InitWaitingInterruptTasksTable();
}

static Task *
RegisterTask(Lz_TaskConfiguration * const taskConfiguration)
{
  HpfTask *newTask = KIncrementalMalloc(sizeof(HpfTask));
  if (NULL == newTask) {
    Panic();
  }

  newTask->priority = taskConfiguration->priority;
  List_InitLinkedListElement(&(newTask->stateQueue));
  InsertTaskByPriority(&readyTasks, newTask);

  return &newTask->base;
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
  Arch_StartRunning(currentTask->stackPointer,
                    OFFSET_OF(pc, TaskContextLayout));
}

static void
HandleInterrupt(void * const sp, const uint8_t interruptCode)
{
  LinkedListElement *first;
  HpfTask *waitingTask;

  first = List_PickFirst(&waitingInterruptTasks[interruptCode]);
  if (NULL == first) {
    /* No task is waiting for that interrupt, restore the running task. */
    Arch_RestoreContextAndReturnFromInterrupt(sp);
  }

  waitingTask = CONTAINER_OF(first, stateQueue, HpfTask);

  if (waitingTask->priority <= ((HpfTask*)currentTask)->priority) {
    /*
     * A task is waiting for that interrupt, but at a lower priority that the
     * running task.
     * Set the waiting task to be ready to execute, according to its priority.
     * Then restore the running task.
     */
    InsertTaskByPriority(&readyTasks, waitingTask);
    Arch_RestoreContextAndReturnFromInterrupt(sp);
  }

  /*
   * If we are here, the task waiting for that interrupt has a higher priority
   * than the running task.
   * We push the running task on top of the stack of waiting tasks, and we run
   * the waiting task.
   */
  currentTask->stackPointer = sp;

  List_Prepend(&readyTasks, &(((HpfTask*)currentTask)->stateQueue));

  currentTask = (Task*)waitingTask;

  Arch_RestoreContextAndReturnFromInterrupt(currentTask->stackPointer);
}

static void
WaitEvent(void * const sp, const uint8_t eventCode)
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

  Arch_RestoreContextAndReturnFromInterrupt(currentTask->stackPointer);
}

/**
 * Defines the operations of the Highest Priority First scheduler.
 */
const SchedulerOperations HPFSchedulerOperations = {
  Init,             /**< member: init                    */
  RegisterTask,     /**< member: registerTask            */
  Run,              /**< member: run                     */
  HandleInterrupt,  /**< member: handleInterrupt         */
  WaitEvent,        /**< member: waitEvent               */
  NULL,             /**< member: wakeupTasksWaitingMutex */
  NULL              /**< member: waitMutex               */
};

/** @} */

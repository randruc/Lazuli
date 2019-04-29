/**
 * @file src/kern/scheduler_base.c
 * @brief Base scheduler implementation.
 * @date Mar 2018
 * @author Remi Andruccioli
 *
 * This files describes the common implementation of every scheduler classes in
 * Lazuli.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler_base.h>
#include <Lazuli/sys/scheduler_hpf.h>
#include <Lazuli/sys/scheduler_rr.h>

Task *currentTask;

/* TODO: Maybe think about storing default task configuration in progmem */
/**
 * Contains default values for Lz_TaskConfiguration.
 */
static const Lz_TaskConfiguration DefaultTaskConfiguration = {
  NULL                                             /**< member: name      */,
  LZ_CONFIG_DEFAULT_TASK_STACK_SIZE                /**< member: stackSize */,
  (Lz_TaskPriority)LZ_CONFIG_DEFAULT_TASK_PRIORITY /**< member: priority  */
};

/**
 * Contains the scheduling class chosen by the user.
 */
static enum Lz_SchedulerClass schedulerClass;

/**
 * Jump table used to jump to the right scheduler, by way of the SchedulerClass
 * value.
 *
 * This table MUST be ordered by value of enum Lz_SchedulerClass.
 *
 * Here we statically register operations for all register classes.
 */
static const SchedulerOperations *JumpToScheduler[] = {

#if LZ_CONFIG_USE_SCHEDULER_RR
  &RRSchedulerOperations,  /**< index: LZ_SCHED_RR  */
#endif /* LZ_CONFIG_USE_SCHEDULER_RR */

#if LZ_CONFIG_USE_SCHEDULER_HPF
  &HPFSchedulerOperations, /**< index: LZ_SCHED_HPF */
#endif /* LZ_CONFIG_USE_SCHEDULER_HPF */

};

/** @cond false */
STATIC_ASSERT
(__LZ_SCHEDULERCLASS_ENUM_END == ELEMENTS_COUNT(JumpToScheduler),
 The_JumpToScheduler_table_must_refer_each_Lz_SchedulerClass_enum_entry);
/** @endcond */

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

  contextLayout->pc = ReverseBytesOfFunctionPointer(task->entryPoint);
  task->stackPointer = ALLOW_ARITHM((void*)contextLayout) - 1;
}

void
BaseScheduler_Init(void)
{
  JumpToScheduler[schedulerClass]->init();
}

void
BaseScheduler_HandleInterrupt(void * const sp, const uint8_t interruptCode)
{
  if (LZ_CONFIG_CHECK_INTERRUPT_CODE_OVER_LAST_ENTRY) {
    if (interruptCode > INT_LAST_ENTRY) {
      Panic();
    }
  }

  JumpToScheduler[schedulerClass]->handleInterrupt(sp, interruptCode);
}

/* TODO: Maybe think about rename this one to WaitInterrupt */
void
BaseScheduler_WaitEvent(void * const sp, const uint8_t eventCode)
{
  JumpToScheduler[schedulerClass]->waitEvent(sp, eventCode);
}

void
BaseScheduler_WakeupTasksWaitingMutex(LinkedList * const waitingTasks)
{
  JumpToScheduler[schedulerClass]->wakeupTasksWaitingMutex(waitingTasks);
}

void
BaseScheduler_WaitMutex(void * const sp, LinkedList * const waitingTasks)
{
  JumpToScheduler[schedulerClass]->waitMutex(sp, waitingTasks);
}

/** @name User API */
/** @{             */

void
Lz_SetSchedulerClass(const enum Lz_SchedulerClass userSchedulerClass)
{
  schedulerClass = userSchedulerClass;

  /*
   * TODO: This can't stay here.
   * Find the appropriate place to call BaseScheduler_Init();
   */
  BaseScheduler_Init();
}

void
Lz_InitTaskConfiguration(Lz_TaskConfiguration * const taskConfiguration)
{
  if (NULL == taskConfiguration) {
    return;
  }

  MemoryCopy(&DefaultTaskConfiguration,
             taskConfiguration,
             sizeof(Lz_TaskConfiguration));
}

void
Lz_RegisterTask(void (* const taskEntryPoint)(void),
                Lz_TaskConfiguration * const taskConfiguration)
{
  Task *newTask;
  const Lz_TaskConfiguration *configuration;
  void *taskStack;
  size_t desiredStackSize;

  if (NULL == taskConfiguration) {
    configuration = &DefaultTaskConfiguration;
  } else {
    configuration = taskConfiguration;
  }

  newTask = JumpToScheduler[schedulerClass]->registerTask(taskConfiguration);

  desiredStackSize = configuration->stackSize
    /* We add enough space to contain the context of a task on the stack */
    + sizeof(TaskContextLayout)
    /* Plus 1 call to save_context_on_stack (in startup.S) */
    + sizeof(void *);

  taskStack = KIncrementalMalloc(desiredStackSize);
  if (NULL == taskStack) {
    Panic();
  }

  newTask->name = configuration->name;
  newTask->entryPoint = taskEntryPoint;
  newTask->stackPointer = ALLOW_ARITHM(taskStack) + desiredStackSize - 1;

  PrepareTaskContext(newTask);
}

void
Lz_Run(void)
{
  JumpToScheduler[schedulerClass]->run();
}

const char *
Lz_GetTaskName(void)
{
  return currentTask->name;
}

/** @} */

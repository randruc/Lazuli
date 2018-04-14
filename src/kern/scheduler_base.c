/**
 * @file src/kern/scheduler_base.c
 * @brief Base scheduler implementation.
 * @date Mar 2018
 * @author Remi Andruccioli
 *
 * This files describes the common implementation of every scheduler classes in
 * Lazuli.
 */

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

#include <Lazuli/sys/scheduler_base.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/scheduler_rr.h>
#include <Lazuli/sys/scheduler_hpf.h>

/* TODO: Maybe think about storing default task configuration in progmem */
const Lz_TaskConfiguration DefaultTaskConfiguration = {
  NULL                    /**< member: name      */,
  DEFAULT_TASK_STACK_SIZE /**< member: stackSize */,
  (Lz_TaskPriority)64     /**< member: priority  */
};

/**
 * Contains the scheduling class choosed by the user.
 */
static Lz_SchedulerClass schedulerClass;

/**
 * Jump table used to jump to the right scheduler, by way of the SchedulerClass
 * value.
 *
 * This table MUST be ordered by value of enum Lz_SchedulerClass.
 *
 * Here we statically register operations for all register classes.
 */
static const SchedulerOperations *JumpToScheduler[] = {
  &RRSchedulerOperations,  /**< index: LZ_SCHED_RR  */
  &HPFSchedulerOperations, /**< index: LZ_SCHED_HPF */
};

STATIC_ASSERT
(__LZ_SCHEDULERCLASS_ENUM_END == ELEMENTS_COUNT(JumpToScheduler),
 The_JumpToScheduler_table_must_refer_each_Lz_SchedulerClass_enum_entry);

Task *currentTask;

void
BaseSchedulerInit()
{
  JumpToScheduler[schedulerClass]->init();
}

void
BaseSchedulerHandleInterrupt(void * const sp, const u8 interruptCode)
{
  JumpToScheduler[schedulerClass]->handleInterrupt(sp, interruptCode);
}

void
BaseSchedulerWaitEvent(void * const sp, const u8 eventCode)
{
  JumpToScheduler[schedulerClass]->waitEvent(sp, eventCode);
}

void
BaseSchedulerPrepareTaskContext(Task * const task)
{
  TaskContextLayout * const contextLayout
    = (TaskContextLayout *)(ALLOW_ARITHM(task->stackPointer)
                            - sizeof(TaskContextLayout) + 1);

  contextLayout->pc = (FuncVoidVoid)swap16((u16)task->entryPoint);
  task->stackPointer = ALLOW_ARITHM((void*)contextLayout) - 1;
}

/** @name User API */
/** @{             */

void
Lz_SetSchedulerClass(const Lz_SchedulerClass userSchedulerClass)
{
  schedulerClass = userSchedulerClass;
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
Lz_RegisterTask(void (* const taskEntryPoint)(),
                 Lz_TaskConfiguration * const taskConfiguration)
{
  JumpToScheduler[schedulerClass]->registerTask(taskEntryPoint,
                                                taskConfiguration);
}

void
Lz_Run()
{
  JumpToScheduler[schedulerClass]->run();
}

const char *
Lz_GetTaskName()
{
  return currentTask->name;
}

/** @} */

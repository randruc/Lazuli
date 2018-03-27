#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

#include <Lazuli/sys/config.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler_base.h>
#include <Lazuli/sys/scheduler_rr.h>

/* TODO: Maybe think about storing default task configuration in progmem */
const Lz_TaskConfiguration DefaultTaskConfiguration = {
  NULL                    /**< member: name      */,
  DEFAULT_TASK_STACK_SIZE /**< member: stackSize */,
  (u16)0                  /**< member: T         */,
  (u16)0                  /**< member: C         */,
  (u16)0                  /**< member: D         */
};

/**
 * Contains the scheduling class choosed by the user.
 */
static Lz_SchedulerClass schedulerClass;

/**
 * Jump table used to jump to the right scheduler, by way of the SchedulerClass
 * value.
 *
 * This table MUST be order by value of enum Lz_SchedulerClass.
 *
 * Here we statically register operations for all register classes.
 */
static const SchedulerOperations *JumpToScheduler[] = {
  &RRSchedulerOperations /**< index: LZ_SCHED_RR */
};

STATIC_ASSERT
(__LZ_SCHEDULERCLASS_ENUM_END == ELEMENTS_COUNT(JumpToScheduler),
 The_JumpToScheduler_table_must_refer_each_Lz_SchedulerClass_enum_entry);

Task *currentTask;

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

const char *
Lz_GetTaskName()
{
  return currentTask->name;
}

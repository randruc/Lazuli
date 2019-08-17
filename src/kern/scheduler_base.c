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
#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler_base.h>
#include <Lazuli/sys/scheduler_rms.h>

Task *currentTask;

/**
 * Contains default values for Lz_TaskConfiguration.
 */
static PROGMEM const Lz_TaskConfiguration DefaultTaskConfiguration = {
  NULL                                             /**< member: name       */,
  LZ_CONFIG_DEFAULT_TASK_STACK_SIZE                /**< member: stackSize  */,
  (Lz_TaskPriority)LZ_CONFIG_DEFAULT_TASK_PRIORITY /**< member: priority   */,
  0                                                /**< member: period     */,
  0                                                /**< member: completion */,
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
  &RMSSchedulerOperations, /** < index: LZ_SCHED_RMS */
};

/** @cond false */
STATIC_ASSERT
(__LZ_SCHEDULERCLASS_ENUM_END == ELEMENTS_COUNT(JumpToScheduler),
 The_JumpToScheduler_table_must_refer_each_Lz_SchedulerClass_enum_entry);
/** @endcond */

/*
 * We could declare this function as static.
 * But if we do this we can't unit test it...
 */
void
(*ReverseBytesOfFunctionPointer(void (* const pointer)(void)))(void)
{
  const uint8_t maxIndex = sizeof(pointer) - 1;
  const uint8_t * const oldPointerPointer = (const uint8_t * const)&pointer;
  void (*newPointer)(void);
  uint8_t * const newPointerPointer = (uint8_t * const)&newPointer;
  uint8_t i;

  for (i = 0; i <= maxIndex; ++i) {
    newPointerPointer[maxIndex - i] = oldPointerPointer[i];
  }

  return newPointer;
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

  contextLayout->pc = ReverseBytesOfFunctionPointer(task->entryPoint);
  contextLayout->terminationCallback =
    ReverseBytesOfFunctionPointer(Lz_Task_Terminate);

  task->stackPointer = ALLOW_ARITHM((void*)contextLayout) - 1;
}

void
BaseScheduler_Init(void)
{
  JumpToScheduler[schedulerClass]->init();
}

void
BaseScheduler_ManageTaskTermination(void * const sp)
{
  JumpToScheduler[schedulerClass]->manageTaskTermination(sp);
}

void
BaseScheduler_AbortTask(void * const sp)
{
  JumpToScheduler[schedulerClass]->abortTask(sp);
}

void
BaseScheduler_HandleInterrupt(void * const sp, const uint8_t interruptCode)
{
  if (LZ_CONFIG_CHECK_INTERRUPT_CODE_OVER_LAST_ENTRY) {
    if (interruptCode > INT_LAST_ENTRY) {
      Kernel_Panic();
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
BaseScheduler_WakeupTasksWaitingMutex(Lz_LinkedList * const waitingTasks)
{
  JumpToScheduler[schedulerClass]->wakeupTasksWaitingMutex(waitingTasks);
}

void
BaseScheduler_WaitMutex(void * const sp, Lz_LinkedList * const waitingTasks)
{
  JumpToScheduler[schedulerClass]->waitMutex(sp, waitingTasks);
}

bool
BaseScheduler_RegisterTask(void (* const taskEntryPoint)(void),
                           const Lz_TaskConfiguration * taskConfiguration,
                           bool idleTask)
{
  Lz_TaskConfiguration defaultConfiguration;
  Task *newTask;
  void *taskStack;
  size_t desiredStackSize;

  if (NULL == taskConfiguration) {
    Arch_LoadFromProgmem(&DefaultTaskConfiguration,
                         &defaultConfiguration,
                         sizeof(Lz_TaskConfiguration));
    taskConfiguration = &defaultConfiguration;
  }

  newTask = JumpToScheduler[schedulerClass]->registerTask(taskConfiguration,
                                                          idleTask);
  if (NULL == newTask) {
    return false;
  }

  desiredStackSize = taskConfiguration->stackSize
    /* We add enough space to contain the context of a task on the stack */
    + sizeof(TaskContextLayout)
    /* Plus 1 call to save_context_on_stack (in startup.S) */
    + sizeof(void *);

  taskStack = KIncrementalMalloc(desiredStackSize);
  if (NULL == taskStack) {
    return false;
  }

  newTask->name = taskConfiguration->name;
  newTask->entryPoint = taskEntryPoint;
  newTask->stackSize = desiredStackSize;
  newTask->stackOrigin = ALLOW_ARITHM(taskStack) + desiredStackSize - 1;
  newTask->stackPointer = ALLOW_ARITHM(taskStack) + desiredStackSize - 1;

  PrepareTaskContext(newTask);

  return true;
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
Lz_TaskConfiguration_Init(Lz_TaskConfiguration * const taskConfiguration)
{
  if (NULL == taskConfiguration) {
    return;
  }

  Arch_LoadFromProgmem(&DefaultTaskConfiguration,
                       taskConfiguration,
                       sizeof(Lz_TaskConfiguration));
}

bool
Lz_RegisterTask(void (* const taskEntryPoint)(void),
                const Lz_TaskConfiguration * taskConfiguration)
{
  return BaseScheduler_RegisterTask(taskEntryPoint, taskConfiguration, false);
}

void
Lz_Run(void)
{
  JumpToScheduler[schedulerClass]->run();
}

const char *
Lz_Task_GetName(void)
{
  return currentTask->name;
}

/** @} */

/**
 * @file src/include/Lazuli/sys/scheduler_base.h
 * @brief Base scheduler interface. The kernel API of the base scheduler.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file describes the common interface of every scheduler classes in
 * Lazuli.
 * It defines the interface to which each scheduler class must conform.
 * It also defines symbols that are accessible to any scheduler class.
 */

#ifndef LZ_SYS_SCHEDULER_BASE_H
#define LZ_SYS_SCHEDULER_BASE_H

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

#include <Lazuli/sys/task.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents all possible operations common to all scheduler classes.
 */
typedef struct {
  /**
   * Initialize the scheduler.
   */
  void (*init)();

  /**
   * Register a new task.
   *
   * @param taskEntryPoint The entry point of the task to register.
   *                       i.e. A pointer to the function representing the task.
   * @param taskConfiguration A pointer to an Lz_TaskConfiguration containing
   *                          the configuration of the task being registered.
   *                          If NULL is passed, then default values are applied
   *                          for all parameters.
   */
  void (*registerTask)(void (* const taskEntryPoint)(),
                       Lz_TaskConfiguration * const taskConfiguration);

  /**
   * Run the scheduler.
   *
   * Start scheduling tasks.
   */
  void (*run)();

  /**
   * Entry point of the interrupt handler for this scheduler.
   *
   * @param sp The stack pointer of the task being executed before the
   *           interrupt.
   *           This is used to save the task's context address.
   * @param interruptCode The code of the interrupt that occured.
   */
  void (*handleInterrupt)(void * const sp, const u8 interruptCode);

  /**
   * Entry point of the wait handler for this scheduler.
   *
   * @param sp The stack pointer of the task calling the wait routine.
   * @param eventCode The code of the wait event.
   */
  void (*waitEvent)(void * const sp, const u8 eventCode);
}SchedulerOperations;

/**
 * A pointer to the current running task.
 */
extern Task *currentTask;

/**
 * Contains default values for Lz_TaskConfiguration.
 */
extern const Lz_TaskConfiguration DefaultTaskConfiguration;

/**
 * Initialize the scheduler prior to running it.
 * This function is called by kernel initialization.
 */
void
BaseSchedulerInit();

/*
 * TODO: Maybe rename interruptCode and eventCode to xxxxxId or something like
 * that...
 */
/**
 * This function is called by arch-specific interrupt handling routine, after
 * saving the context of the current running task.
 *
 * @param sp The stack pointer of the current running task after saving its
 *           context.
 * @param interruptCode The code of the interrupt being handled.
 */
void
BaseSchedulerHandleInterrupt(void * const sp, const u8 interruptCode);

/**
 * This function is called from arch-specific Wait routine in order to get the
 * current running task wait for a particular event, after saving its context.
 *
 * @param sp The stack pointer of the current running task after saving its
 *           context.
 * @param eventCode The code of the event the task is waiting for.
 */
void
BaseSchedulerWaitEvent(void * const sp, const u8 eventCode);

/**
 * Prepare the first context of the task so it will be ready when switching
 * context for the first time (i.e. run the scheduler).
 *
 * @param task A pointer to the Task to prepare.
 */
void
BaseSchedulerPrepareTaskContext(Task * const task);

_EXTERN_C_DECL_END

#endif /* LZ_SYS_SCHEDULER_BASE_H */

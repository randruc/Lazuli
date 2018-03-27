/**
 * @file src/include/Lazuli/sys/scheduler_base.h
 * @brief The kernel API of the base scheduler.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file describes the kernel API of the base scheduler.
 * These symbols are common to all classes of schedulers in Lazuli.
 * All the symbols described here are only accessible from the kernel.
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
 */
void
BaseSchedulerInit();

_EXTERN_C_DECL_END

#endif /* LZ_SYS_SCHEDULER_BASE_H */

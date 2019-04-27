/**
 * @file src/include/Lazuli/lazuli.h
 * @brief The public API of the Lazuli kernel.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file describes the public API of the Lazuli kernel.
 * It defines public types and functions that can by used by user main code
 * and tasks.
 */

#ifndef LAZULI_LAZULI_H
#define LAZULI_LAZULI_H

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/sys/config.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents a scheduling policy to run.
 */
enum Lz_SchedulerClass {
  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the first one.
   */
  __LZ_SCHEDULERCLASS_ENUM_BEGIN = -1,

  /** @endcond */

#if CONFIG_USE_SCHEDULER_RR
  /**
   * Round-Robin scheduling.
   */
  LZ_SCHED_RR,
#endif /* CONFIG_USE_SCHEDULER_RR */

#if CONFIG_USE_SCHEDULER_HPF
  /**
   * Highest Priority First scheduling.
   */
  LZ_SCHED_HPF,
#endif /* CONFIG_USE_SCHEDULER_HPF */

  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the last one.
   */
  __LZ_SCHEDULERCLASS_ENUM_END

  /** @endcond */
};

/**
 * Represents the priority of a task.
 * The higher the value, the higher the priority.
 */
typedef int8_t Lz_TaskPriority;

/**
 * Represents the configuration of a task.
 */
typedef struct {
  /**
   * A pointer to an allocated const string containing the name to give to the
   * task.
   * That string must NOT be deallocated after registering the task.
   */
  char const *name;

  /**
   * The size of the stack needed by the task
   */
  size_t stackSize;

  /**
   * The priority of the task.
   * Used for HPF scheduling.
   */
  Lz_TaskPriority priority;
}Lz_TaskConfiguration;

/**
 * Set the scheduler class.
 *
 * @param userSchedulerClass A value of enum Lz_SchedulerClass to define the
 *                           scheduling policy.
 */
void
Lz_SetSchedulerClass(const enum Lz_SchedulerClass userSchedulerClass);

/**
 * Register a new task.
 *
 * @param taskEntryPoint The entry point of the task to register.
 *                       i.e. A pointer to the function representing the task.
 * @param taskConfiguration A pointer to an Lz_TaskConfiguration containing the
 *                          configuration of the task being registered.
 *                          If NULL is passed, then default values are applied
 *                          for all parameters.
 */
void
Lz_RegisterTask(void (* const taskEntryPoint)(void),
                Lz_TaskConfiguration * const taskConfiguration);

/**
 * @brief Initialize an Lz_TaskConfiguration with default values for all
 *        parameters.
 *
 * No function is provided for allocating a new Lz_TaskConfiguration.
 * So it is strongly advised to allocate the Lz_TaskConfiguration parameter
 * on the stack before calling this function.
 *
 * @param taskConfiguration A pointer to the Lz_TaskConfiguration to initialize.
 */
void
Lz_InitTaskConfiguration(Lz_TaskConfiguration * const taskConfiguration);

/**
 * Run the scheduler.
 *
 * Start scheduling tasks.
 */
void
Lz_Run(void);

/**
 * Wait for a specific interrupt to occur.
 * Puts the calling task to sleep until the specified interrupt occurs.
 *
 * @param interruptCode The code of the interrupt to wait for.
 */
void
Lz_WaitInterrupt(uint8_t interruptCode);

/**
 * Get the name of the calling task.
 *
 * @return A pointer to a string containing the name of the current running
 *         task, or NULL if the task has no name.
 */
char const *
Lz_GetTaskName(void);

_EXTERN_C_DECL_END

#endif /* LAZULI_LAZULI_H */

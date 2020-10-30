/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief The public API of the Lazuli kernel.
 * @copyright 2017-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the public (user) API of the Lazuli kernel.
 * It defines public types and functions that can by used by user main code
 * and tasks.
 */

#ifndef LAZULI_LAZULI_H
#define LAZULI_LAZULI_H

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents the priority of a task, as a signed integer.
 * The higher the value, the higher the priority.
 */
typedef int8_t lz_task_priority_t;

/**
 * Represents the type used for the system clock resolution unit, as an unsigned
 * integer.
 */
typedef uint16_t lz_u_resolution_unit_t;

/**
 * Represents the type used for scheduling policies of a Lazuli user task.
 */
typedef uint8_t lz_scheduling_policy_t;

/*
 * TODO: See if we should rename the policies macro constants as LZ_POLICY_XXX
 *       or LZ_SCHEDULING_POLICY_XXX.
 */

/**
 * Cyclic real-time scheduling.
 */
#define CYCLIC_RT ((lz_scheduling_policy_t)0U)

/**
 * Priority time sliced real-time scheduling.
 *
 * Equivalent to POSIX SCHED_RR.
 */
#define PRIORITY_RT ((lz_scheduling_policy_t)1U)

/**
 * Represents the maximum value currently defined for a lz_scheduling_policy_t.
 */
#define LZ_SCHEDULING_POLICY_MAX PRIORITY_RT

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
   * The scheduling policy of the task.
   */
  lz_scheduling_policy_t schedulingPolicy;

  /**
   * The priority of task. Only used for non-cyclic tasks.
   * The lower this number is, the higher the priority will be.
   */
  lz_task_priority_t priority;

  /**
   * The period (T) of the task. Used only for cyclic tasks.
   *
   * The period is expressed as an integer number of time units.
   */
  lz_u_resolution_unit_t period;

  /**
   * The completion time (C) of the task (worst case execution time). Used only
   * for cyclic tasks.
   *
   * The completion time is expressed as an integer number of time units.
   */
  lz_u_resolution_unit_t completion;
}Lz_TaskConfiguration;

/**
 * Register a new task.
 *
 * If an error occurred during registration of the task _false_ is returned and
 * the task is not included in the set of tasks that will be run.
 *
 * @param taskEntryPoint The entry point of the task to register.
 *                       i.e. A pointer to the function representing the task.
 * @param taskConfiguration A pointer to an Lz_TaskConfiguration containing the
 *                          configuration of the task being registered.
 *                          If NULL is passed, then default values are applied
 *                          for all parameters.
 *
 * @return
 *         - _true_ if the task has been registered without error.
 *         - _false_ if an error occurred during registration.
 */
bool
Lz_RegisterTask(void (* const taskEntryPoint)(void),
                Lz_TaskConfiguration * taskConfiguration);

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
Lz_TaskConfiguration_Init(Lz_TaskConfiguration * const taskConfiguration);

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
 *
 * @attention Only tasks with scheduling policy PRIORITY_RT can wait for
 *            interrupts.
 */
void
Lz_Task_WaitInterrupt(uint8_t interruptCode);

/**
 * Get the name of the calling task.
 *
 * @return A pointer to a string containing the name of the current running
 *         task, or NULL if the task has no name.
 */
char const *
Lz_Task_GetName(void);

/**
 * Terminate the calling task. The context of the task will be saved on its
 * stack.
 *
 * Calling this function has the same effect than returning from the task's main
 * function.
 *
 * The terminated task will never be scheduled again.
 */
void
Lz_Task_Terminate(void);

/**
 * Set the calling task to wait for its next activation.
 * May be used if the task finished its work without consuming all of its
 * completion time.
 *
 * @attention Only tasks with scheduling policy CYCLIC_RT can wait for next
 *            activation.
 */
void
Lz_Task_WaitActivation(void);

/**
 * Set the calling task to wait for the specified number of time resolution
 * units (time slices), using the software timer.
 * As Lazuli is a time sliced operating system, the effective waiting will start
 * at the end of the current time slice. This means that the real waiting time
 * _starting from the calling of this function_ will be:
 *
 * units / clock resolution frequency <= waiting time
 * **AND**
 * waiting time < (units + 1) / clock resolution frequency
 *
 * See the configuration option LZ_CONFIG_SYSTEM_CLOCK_RESOLUTION_FREQUENCY.
 *
 * @param units The number of time slices to wait.
 *
 * @warning Only works for tasks with PRIORITY_RT policy.
 */
void
Lz_WaitTimer(lz_u_resolution_unit_t units);

_EXTERN_C_DECL_END

#endif /* LAZULI_LAZULI_H */

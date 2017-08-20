/**
 * @file src/include/Lazuli/scheduler.h
 *
 * This file describes the interface of the scheduler.
 */

#ifndef LZ_SCHEDULER_H
#define LZ_SCHEDULER_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents the configuration of a task being registered in the scheduler.
 */
typedef struct {
  size_t stackSize; /**< The size of the stack needed by the task */
}TaskConfiguration;

/**
 * Register a new task.
 *
 * @param taskEntryPoint The entry point of the task to register.
 *                       i.e. A pointer to the function representing the task.
 * @param taskConfiguration A pointer to a TaskConfiguration containing the
 *                          configuration of the task being registered.
 *                          If NULL is passed, then default values are applied
 *                          for all parameters.
 */
void
Lz_Scheduler_RegisterTask(void (*taskEntryPoint)(),
                          TaskConfiguration * const taskConfiguration);

/**
 * Initialize a TaskConfiguration with default values for all parameters.
 *
 * No function is provided for allocating a new TaskConfiguration. The user is
 * responsible of the creation of a new TaskConfiguration.
 *
 * @param taskConfiguration A pointer to the TaskConfiguration to initialize.
 */
void
Lz_Scheduler_InitTaskConfiguration(TaskConfiguration * const taskConfiguration);

/**
 * Run the scheduler.
 */
void
Lz_Scheduler_Run();

/**
 * Wait for the interrupt "External Interrupt Request 0".
 * Blocks the calling task until "External Interrupt Request 0" occurs.
 */
void
Lz_WaitInt0();

_EXTERN_C_DECL_END

#endif /* LZ_SCHEDULER_H */

/**
 * @file scheduler.h
 *
 * This file describes the interface of the scheduler.
 */

#ifndef LZ_SCHEDULER_H
#define LZ_SCHEDULER_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Register a new task.
 *
 * @param taskEntryPoint The entry point of the task to register.
 *                       i.e. A pointer to the function representing the task.
 */
void
Lz_Scheduler_RegisterTask(void (*taskEntryPoint)());

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

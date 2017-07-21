/**
 * @file scheduler.h
 *
 * This file describes the kernel interface for the scheduler.
 */

#ifndef LZ_SYS_SCHEDULER_H
#define LZ_SYS_SCHEDULER_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Initialize the scheduler.
 */
void
Scheduler_Init();

_EXTERN_C_DECL_END

#endif /* LZ_SYS_SCHEDULER_H */

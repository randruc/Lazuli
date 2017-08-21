/**
 * @file src/include/Lazuli/sys/scheduler.h
 * @brief The private API of the scheduler.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file describes the private API of the scheduler.
 * All the symbols described here are only accessible from the kernel.
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

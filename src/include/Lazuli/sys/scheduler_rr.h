/**
 * @file src/include/Lazuli/sys/scheduler_rr.h
 * @brief Round-Robin tasks scheduler symbols.
 * @date Mar 2018
 * @author Remi Andruccioli
 *
 * This file defines the externally-visible symbols from RR scheduler.
 */

#ifndef LZ_SYS_SCHEDULER_RR_H
#define LZ_SYS_SCHEDULER_RR_H

#include <Lazuli/common.h>
#include <Lazuli/sys/scheduler_base.h>

_EXTERN_C_DECL_BEGIN

/**
 * Jump table to the operations of the Round-Robin scheduler.
 */
extern const SchedulerOperations RRSchedulerOperations;

_EXTERN_C_DECL_END

#endif /* LZ_SYS_SCHEDULER_RR_H */

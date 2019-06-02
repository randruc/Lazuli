/**
 * @file src/include/Lazuli/sys/scheduler_rr.h
 * @brief Round-Robin tasks scheduler global symbols.
 * @date Mar 2018
 * @author Remi Andruccioli
 *
 * This file defines the externally-visible symbols from RR scheduler.
 */

#ifndef LAZULI_SYS_SCHEDULER_RR_H
#define LAZULI_SYS_SCHEDULER_RR_H

#include <Lazuli/common.h>
#include <Lazuli/sys/scheduler_base.h>

_EXTERN_C_DECL_BEGIN

/**
 * Jump table to the operations of the Round-Robin scheduler.
 */
extern const SchedulerOperations RRSchedulerOperations;

/**
 * Represents a task registered by the RR scheduler.
 */
typedef struct {
  /**
   * The base definition of a task.
   *
   * @warning This MUST be the first member of the struct.
   */
  Task base;

  /**
   * The scheduling queue on which the task is stored.
   */
  Lz_LinkedListElement stateQueue;
}RrTask;

/**
 * @cond false
 *
 * TODO: Find out why we have to write !XXX instead of XXX == 0
 */
STATIC_ASSERT(!OFFSET_OF(base, RrTask),
              The_member_base_must_be_at_offset_0_in_struct_RrTask);
/** @endcond */

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_SCHEDULER_RR_H */

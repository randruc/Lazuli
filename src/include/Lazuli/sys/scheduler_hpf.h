/**
 * @file src/include/Lazuli/sys/scheduler_hpf.h
 * @brief Highest Priority First scheduler global symbols.
 * @date Apr 2018
 * @author Remi Andruccioli
 *
 * This file defines the externally-visible symbols from the HPF scheduler.
 */

#ifndef LAZULI_SYS_SCHEDULER_HPF_H
#define LAZULI_SYS_SCHEDULER_HPF_H

#include <Lazuli/common.h>
#include <Lazuli/sys/scheduler_base.h>
#include <Lazuli/sys/task.h>

_EXTERN_C_DECL_BEGIN

/**
 * Jump table to the operations of the Highest Priority First scheduler.
 */
extern const SchedulerOperations HPFSchedulerOperations;

/**
 * Represents a task registered by the HPF scheduler.
 */
typedef struct {
  /* WARNING: This MUST be the first member of the struct */
  /** The base definition of a task */
  Task base;

  /** The task priority */
  Lz_TaskPriority priority;

  /** The scheduling queue on which the task is stored */
  LinkedListElement stateQueue;
}HpfTask;

/* TODO: Find out why we have to write !XXX instead of XXX == 0 */
STATIC_ASSERT(!OFFSET_OF(base, HpfTask),
              The_member_base_must_be_at_offset_0_in_struct_HpfTask);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_SCHEDULER_HPF_H */

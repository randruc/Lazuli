/**
 * @file src/include/Lazuli/sys/scheduler_rms.h
 * @brief Rate Monotonic Scheduler global symbols.
 * @date May 2019
 * @author Remi Andruccioli
 *
 * This file defines the externally-visible symbols from RMS scheduler.
 */

#ifndef LAZULI_SYS_SCHEDULER_RMS_H
#define LAZULI_SYS_SCHEDULER_RMS_H

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/list.h>
#include <Lazuli/sys/scheduler.h>

_EXTERN_C_DECL_BEGIN

/**
 * Jump table to the operations of the Rate Monotonic Scheduler.
 */
extern const SchedulerOperations RMSSchedulerOperations;

/**
 * Represents a message that an RmsTask can pass to the scheduler after its time
 * slice has expired.
 */
enum TaskToSchedulerMessage {
  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the first one.
   */
  __TASK_TO_SCHEDULER_MESSAGE_ENUM_BEGIN = -1,

  /**
   * No message has to be passed to the scheduler.
   */
  NO_MESSAGE = -1,

  /**
   * Set the task to wait for its next activation.
   * i.e. It finnished its work without consuming all of its completion time.
   */
  WAIT_ACTIVATION,

  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the last one.
   */
  __TASK_TO_SCHEDULER_MESSAGE_ENUM_END
};

/**
 * Represents a task registered in the RMS scheduler.
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

  /**
   * The period (T) of the task, expressed as an integer number of time units.
   * Defined by task configuration when registering task, then left read-only.
   */
  Lz_ResolutionUnit period;

  /**
   * The completion time (C) of the task (worst case execution time), expressed
   * as an integer number of time units.
   * Defined by task configuration when registering task, then left read-only.
   */
  Lz_ResolutionUnit completion;

  /**
   * The number of time units until the task will complete its execution.
   * Updated by scheduler.
   */
  Lz_ResolutionUnit timeUntilCompletion;

  /**
   * The number of time units until the task will be activated.
   * Updated by scheduler.
   */
  Lz_ResolutionUnit timeUntilActivation;

  /**
   * The message the task has to pass to the scheduler for the next scheduling
   * operation (i.e. after its time slice expires).
   */
  enum TaskToSchedulerMessage taskToSchedulerMessage;
}RmsTask;

/**
 * @cond false
 *
 * TODO: Find out why we have to write !XXX instead of XXX == 0
 */
STATIC_ASSERT(!OFFSET_OF(base, RmsTask),
              The_member_base_must_be_at_offset_0_in_struct_RmsTask);
/** @endcond */

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_SCHEDULER_RMS_H */

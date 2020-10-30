/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Base task definition and context description.
 * @copyright 2017-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * Describes all types and functions related to tasks.
 */

#ifndef LAZULI_SYS_TASK_H
#define LAZULI_SYS_TASK_H

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/list.h>

_EXTERN_C_DECL_BEGIN

/**
 * Defines the type used to contain a message that a Task can pass to the
 * scheduler after its time slice has expired.
 *
 * This type is declared read/write atomic because in the definition of
 * struct Task a message can come along with data, pointed by
 * taskToSchedulerMessageParameter. By writing the pointer first, then writing
 * the message code, we can ensure the integrity of the full message
 * (i.e.: message code + data), assuming the execution is "in-order".
 */
typedef u_read_write_atomic_t lz_task_to_scheduler_message_t;

/**
 * No message has to be passed to the scheduler.
 */
#define NO_MESSAGE ((lz_task_to_scheduler_message_t)0U)

/**
 * Set the task to wait for its next activation.
 * i.e. It finished its work without consuming all of its completion time.
 */
#define WAIT_ACTIVATION ((lz_task_to_scheduler_message_t)1U)

/**
 * Set the task to wait for an interrupt.
 * A parameter representing the interrupt number must accompany this message.
 */
#define WAIT_INTERRUPT ((lz_task_to_scheduler_message_t)2U)

/**
 * Terminate the task. This handles the case of a normal termination of a task.
 */
#define TERMINATE_TASK ((lz_task_to_scheduler_message_t)3U)

/**
 * Wait for a mutex to be unlocked.
 * A parameter pointing to the mutex must accompany this message.
 */
#define WAIT_MUTEX ((lz_task_to_scheduler_message_t)4U)

/**
 * Set the task to wait for the specified number of time resolution units,
 * using the software timer.
 * A parameter pointing to the specified number of units must accompany this
 * message.
 */
#define WAIT_SOFTWARE_TIMER ((lz_task_to_scheduler_message_t)5U)

/**
 * Abort the curent running task. Called by the kernel if some unrecoverable
 * error occured (e.g. A division by zero).
 */
#define ABORT_TASK ((lz_task_to_scheduler_message_t)6U)

/**
 * Represents a task.
 */
typedef struct {
  /**
   * The name of the task.
   *
   * > Never changes once the Task is allocated.
   */
  const char *name;

  /**
   * Entry point of execution of the task.
   *
   * > Never changes once the Task is allocated.
   */
  void (*entryPoint)(void);

  /**
   * The bottom of the allocated stack for the task.
   *
   * > Never changes once the Task is allocated.
   *
   * @warning This member always points to the bottom of the task's stack (the
   *          first byte of the stack).
   *          So if the stack grows downwards (as on many architectures) this
   *          will always point to an address higher or equal to member
   *          stackPointer.
   */
  void *stackOrigin;

  /**
   * The stack size of the task.
   *
   * > Never changes once the Task is allocated.
   */
  size_t stackSize;

  /**
   * The saved stack pointer of the task.
   */
  void *stackPointer;

  /**
   * The scheduling policy.
   */
  lz_scheduling_policy_t schedulingPolicy;

  /**
   * The scheduling queue on which the task is stored.
   */
  Lz_LinkedListElement stateQueue;

  /**
   * The period (T) of the task, expressed as an integer number of time units.
   * Defined by task configuration when registering task, then left read-only.
   */
  lz_u_resolution_unit_t period;

  /**
   * The completion time (C) of the task (worst case execution time), expressed
   * as an integer number of time units.
   * Defined by task configuration when registering task, then left read-only.
   */
  lz_u_resolution_unit_t completion;

  /**
   * The number of time units until the task will complete its execution.
   * Updated by scheduler.
   */
  lz_u_resolution_unit_t timeUntilCompletion;

  /**
   * The number of time units until the task will be activated.
   * Updated by scheduler.
   */
  lz_u_resolution_unit_t timeUntilActivation;

  /**
   * The task priority. Only used for non-cyclic tasks.
   */
  lz_task_priority_t priority;

  /**
   * The number of time units until the software timer expires for the task.
   */
  lz_u_resolution_unit_t timeUntilTimerExpiration;

  /**
   * The message the task has to pass to the scheduler for the next scheduling
   * operation (i.e. after its time slice expires).
   *
   * @attention Declared volatile because it can be updated both by the task
   *            itself or the kernel.
   *
   * @warning This field is declared with a data type that is read/write atomic.
   *          If the field taskToSchedulerMessageParameter is used to pass a
   *          message to the scheduler, then this field taskToSchedulerMessage
   *          must be written **last** to ensure the integrity of all the pieces
   *          of data that compose the message to the scheduler.
   */
  volatile lz_task_to_scheduler_message_t taskToSchedulerMessage;

  /**
   * A parameter that can accompany a taskToSchedulerMessage.
   *
   * This parameter is a "universal pointer" to the actual parameter.
   */
  void *taskToSchedulerMessageParameter;
}Task;

/**
 * This type definition is needed to exclude confusion with the 'volatile'
 * type qualifier used in the struct declaration below.
 */
typedef void (*FuncVoidVoid)(void);

/**
 * Represents the layout of the stack when saving the context of a task.
 *
 * The context of a task consists in saving all of the registers and the program
 * counter.
 *
 * @warning This in fact is a mixed representation of the original layout of the
 *          task and the saved layout of the task. @n
 *          In the original layout (when the task is allocated) we are not
 *          interested in the registers.
 *          And in the saved layout (when the context is saved, e.g. preemption)
 *          we are not interested in the termination callback.
 */
typedef struct {
  /** State register */
  volatile uint8_t sreg;

  /** Register 0 */
  volatile uint8_t r0;

  /** Register 1 */
  volatile uint8_t r1;

  /** Register 2 */
  volatile uint8_t r2;

  /** Register 3 */
  volatile uint8_t r3;

  /** Register 4 */
  volatile uint8_t r4;

  /** Register 5 */
  volatile uint8_t r5;

  /** Register 6 */
  volatile uint8_t r6;

  /** Register 7 */
  volatile uint8_t r7;

  /** Register 8 */
  volatile uint8_t r8;

  /** Register 9 */
  volatile uint8_t r9;

  /** Register 10 */
  volatile uint8_t r10;

  /** Register 11 */
  volatile uint8_t r11;

  /** Register 12 */
  volatile uint8_t r12;

  /** Register 13 */
  volatile uint8_t r13;

  /** Register 14 */
  volatile uint8_t r14;

  /** Register 15 */
  volatile uint8_t r15;

  /** Register 16 */
  volatile uint8_t r16;

  /** Register 17 */
  volatile uint8_t r17;

  /** Register 18 */
  volatile uint8_t r18;

  /** Register 19 */
  volatile uint8_t r19;

  /** Register 20 */
  volatile uint8_t r20;

  /** Register 21 */
  volatile uint8_t r21;

  /** Register 22 */
  volatile uint8_t r22;

  /** Register 23 */
  volatile uint8_t r23;

  /** Register 24 */
  volatile uint8_t r24;

  /** Register 25 */
  volatile uint8_t r25;

  /** Register 26 */
  volatile uint8_t r26;

  /** Register 27 */
  volatile uint8_t r27;

  /** Register 28 */
  volatile uint8_t r28;

  /** Register 29 */
  volatile uint8_t r29;

  /** Register 30 */
  volatile uint8_t r30;

  /** Register 31 */
  volatile uint8_t r31;

  /** Program counter for the task */
  volatile FuncVoidVoid pc;

  /**
   * Callback to the scheduler to manage task termination.
   *
   * @warning This member is valid ONLY at the time when the task is allocated
   *          and never used its stack yet.
   */
  volatile FuncVoidVoid terminationCallback;
}TaskContextLayout;

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_TASK_H */

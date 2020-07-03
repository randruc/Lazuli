/**
 * @file src/include/Lazuli/sys/task.h
 * @brief Base task definition and context description.
 * @author Remi Andruccioli
 * @date Feb 2017
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
 * Represents a message that a Task can pass to the scheduler after its time
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

  /** @endcond */

  /**
   * No message has to be passed to the scheduler.
   */
  NO_MESSAGE = 0,

  /**
   * Set the task to wait for its next activation.
   * i.e. It finnished its work without consuming all of its completion time.
   */
  WAIT_ACTIVATION,

  /**
   * Set the task to wait for an interrupt.
   * A parameter representing the interrupt number must accompany this message.
   */
  WAIT_INTERRUPT,

  /**
   * Terminate the task.
   */
  TERMINATE_TASK,

  /**
   * Wait for a mutex to be unlocked.
   * A parameter representing a pointer to the mutex must accompany this
   * message.
   */
  WAIT_MUTEX,

  /**
   * Set the task to wait for the specified number of time resolution units,
   * using the software timer.
   * A parameter pointing to the specified number of units must accompany this
   * message.
   */
  WAIT_SOFTWARE_TIMER,

  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the last one.
   */
  __TASK_TO_SCHEDULER_MESSAGE_ENUM_END

  /** @endcond */
};

/*
 * TODO: Maybe think about storing scheduling policy parameters in a union to
 * save memory.
 * This would have the drawback of not being able for a task to save its
 * parameters if it needs to alternate between 2 different policies that use
 * different kind of parameters.
 */
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
  enum Lz_SchedulingPolicy schedulingPolicy;

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
   */
  volatile enum TaskToSchedulerMessage taskToSchedulerMessage;

  /**
   * A parameter that can accompany a taskToSchedulerMessage.
   *
   * This parameter is a "universal pointer" to the actual parameter.
   */
  void *taskToSchedulerMessageParameter;
}Task;

/**
 * This type definition is needed to exlude confusion with the 'volatile'
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

/**
 * Abort the calling task.
 *
 * This function can be called when some unrecoverable error occured in the
 * context of a task (e.g. when a mandatory pointer is _NULL_ as a function
 * parameter).
 * This will have the consequence of saving the task context (saving all
 * registers and keeping the call stack) and unscheduling the task.
 */
void
Task_Abort(void);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_TASK_H */

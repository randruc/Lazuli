/**
 * @file src/include/Lazuli/sys/task.h
 *
 * Describes all types and functions related to tasks.
 */

#ifndef LAZULI_SYS_TASK_H
#define LAZULI_SYS_TASK_H

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/list.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents a task.
 *
 * This type is kind of an "abstract" type. It's always nested in a more
 * specific task type, appropriate for each scheduler class.
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

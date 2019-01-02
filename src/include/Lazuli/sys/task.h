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
 */
typedef struct {
  /** The name of the task */
  const char *name;

  /*
   * TODO: See if we can eliminate this member.
   * After all, the PC is set on the task's stack when registering it...
   */
  /** Entry point of execution of the task */
  void (*entryPoint)();

  /** The saved stack pointer of the task */
  void *stackPointer;
}Task;

/**
 * This type definition is needed to exlude confusion with the 'volatile'
 * type qualifier used in the struct declaration below.
 */
typedef void (*FuncVoidVoid)();

/**
 * Represents the layout of the stack when saving the context of a task.
 *
 * The context of a task consists in saving all of the registers and the program
 * counter.
 */
typedef struct {
  volatile uint8_t sreg;          /**< State register               */
  volatile uint8_t r0;            /**< Register 0                   */
  volatile uint8_t r1;            /**< Register 1                   */
  volatile uint8_t r2;            /**< Register 2                   */
  volatile uint8_t r3;            /**< Register 3                   */
  volatile uint8_t r4;            /**< Register 4                   */
  volatile uint8_t r5;            /**< Register 5                   */
  volatile uint8_t r6;            /**< Register 6                   */
  volatile uint8_t r7;            /**< Register 7                   */
  volatile uint8_t r8;            /**< Register 8                   */
  volatile uint8_t r9;            /**< Register 9                   */
  volatile uint8_t r10;           /**< Register 10                  */
  volatile uint8_t r11;           /**< Register 11                  */
  volatile uint8_t r12;           /**< Register 12                  */
  volatile uint8_t r13;           /**< Register 13                  */
  volatile uint8_t r14;           /**< Register 14                  */
  volatile uint8_t r15;           /**< Register 15                  */
  volatile uint8_t r16;           /**< Register 16                  */
  volatile uint8_t r17;           /**< Register 17                  */
  volatile uint8_t r18;           /**< Register 18                  */
  volatile uint8_t r19;           /**< Register 19                  */
  volatile uint8_t r20;           /**< Register 20                  */
  volatile uint8_t r21;           /**< Register 21                  */
  volatile uint8_t r22;           /**< Register 22                  */
  volatile uint8_t r23;           /**< Register 23                  */
  volatile uint8_t r24;           /**< Register 24                  */
  volatile uint8_t r25;           /**< Register 25                  */
  volatile uint8_t r26;           /**< Register 26                  */
  volatile uint8_t r27;           /**< Register 27                  */
  volatile uint8_t r28;           /**< Register 28                  */
  volatile uint8_t r29;           /**< Register 29                  */
  volatile uint8_t r30;           /**< Register 30                  */
  volatile uint8_t r31;           /**< Register 31                  */
  volatile FuncVoidVoid pc;       /**< Program counter for the task */
}TaskContextLayout;

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_TASK_H */

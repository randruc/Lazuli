/**
 * @file src/include/Lazuli/sys/task.h
 *
 * Describes all types and functions related to tasks.
 */

#ifndef LZ_SYS_TASK_H
#define LZ_SYS_TASK_H

#include <Lazuli/common.h>
#include <Lazuli/sys/list.h>

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
  volatile u8 sreg;          /**< State register               */
  volatile u8 r0;            /**< Register 0                   */
  volatile u8 r1;            /**< Register 1                   */
  volatile u8 r2;            /**< Register 2                   */
  volatile u8 r3;            /**< Register 3                   */
  volatile u8 r4;            /**< Register 4                   */
  volatile u8 r5;            /**< Register 5                   */
  volatile u8 r6;            /**< Register 6                   */
  volatile u8 r7;            /**< Register 7                   */
  volatile u8 r8;            /**< Register 8                   */
  volatile u8 r9;            /**< Register 9                   */
  volatile u8 r10;           /**< Register 10                  */
  volatile u8 r11;           /**< Register 11                  */
  volatile u8 r12;           /**< Register 12                  */
  volatile u8 r13;           /**< Register 13                  */
  volatile u8 r14;           /**< Register 14                  */
  volatile u8 r15;           /**< Register 15                  */
  volatile u8 r16;           /**< Register 16                  */
  volatile u8 r17;           /**< Register 17                  */
  volatile u8 r18;           /**< Register 18                  */
  volatile u8 r19;           /**< Register 19                  */
  volatile u8 r20;           /**< Register 20                  */
  volatile u8 r21;           /**< Register 21                  */
  volatile u8 r22;           /**< Register 22                  */
  volatile u8 r23;           /**< Register 23                  */
  volatile u8 r24;           /**< Register 24                  */
  volatile u8 r25;           /**< Register 25                  */
  volatile u8 r26;           /**< Register 26                  */
  volatile u8 r27;           /**< Register 27                  */
  volatile u8 r28;           /**< Register 28                  */
  volatile u8 r29;           /**< Register 29                  */
  volatile u8 r30;           /**< Register 30                  */
  volatile u8 r31;           /**< Register 31                  */
  volatile FuncVoidVoid pc;  /**< Program counter for the task */
}TaskContextLayout;

_EXTERN_C_DECL_END

#endif /* LZ_SYS_TASKS_H */

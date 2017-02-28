/**
 * @file task.h
 *
 * Describes all types and functions related to tasks.
 */

#ifndef LZ_SYS_TASK_H
#define LZ_SYS_TASK_H

#include <Lazuli/common.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/list.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents a task.
 */
typedef struct {
  /** Entry point of execution of the task     */
  void (*entryPoint)();

  /** The state queue on which of the task is stored */
  LinkedListElement stateQueue;

  /** The current stack pointer of the task    */
  void *stackPointer;
}Task;

_EXTERN_C_DECL_END

#endif /* LZ_SYS_TASKS_H */

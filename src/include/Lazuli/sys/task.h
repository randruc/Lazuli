/**
 * @file task.h
 *
 * Describes all types and functions related to tasks.
 */

#ifndef LZ_TASK_H
#define LZ_TASK_H

#include <Lazuli/common.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/list.h>

_EXTERN_C_DECL_BEGIN

/* TODO: Rename this file to task.h */

/**
 * Represents a task.
 */
typedef struct {
  AllocationMap allocationMap; /** < Allocation map of the task           */
  void (*entryPoint)();        /** < Entry point of execution of the task */
  LinkedListElement stateList; /** < List to store the state of the task   */
}Task;

_EXTERN_C_DECL_END

#endif /* LZ_TASKS_H */

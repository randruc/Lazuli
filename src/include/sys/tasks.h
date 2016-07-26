/**
 * @file tasks.h
 *
 * Describes all objects and functions related to tasks.
 */

#ifndef TASKS_H
#define TASKS_H

#include <sys/utils.h>
#include <sys/memory.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents a task.
 */
typedef struct {
  AllocationMap allocationMap; /** < Allocation map for the task           */
  void (*entryPoint)();        /** < Entry point of execution for the task */
}Task;

_EXTERN_C_DECL_END

#endif /* TASKS_H */

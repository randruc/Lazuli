/**
 * @file tasks.h
 *
 * Describes all types and functions related to tasks.
 */

#ifndef LZ_TASKS_H
#define LZ_TASKS_H

#include <Lazuli/common.h>
#include <Lazuli/sys/memory.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents a task.
 */
typedef struct {
  AllocationMap allocationMap; /** < Allocation map for the task           */
  void (*entryPoint)();        /** < Entry point of execution for the task */
}Task;

_EXTERN_C_DECL_END

#endif /* LZ_TASKS_H */

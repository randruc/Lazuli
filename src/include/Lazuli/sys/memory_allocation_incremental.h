/**
 *
 */

#ifndef LAZULI_SYS_MEMORY_ALLOCATION_INCREMENTAL_H
#define LAZULI_SYS_MEMORY_ALLOCATION_INCREMENTAL_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents a map of the allocated memory regions and useful memory handlers
 * for a task or the kernel.
 */
typedef struct {
  /**
   * Base address of the memory region.
   */
  void *baseMem;

  /**
   * Break position, points to the first location beyond the current end of
   * the heap.
   */
  void *brk;

  /**
   * End address of the memory region.
   */
  void *endMem;
}AllocationMap;

/**
 * The allocation map for the kernel.
 */
extern AllocationMap kernelAllocationMap;

/**
 * Allocate memory for kernel objects using incremental method.
 *
 * @param size The size in bytes to allocate.
 *
 * @return A pointer to the allocated region, or NULL if allocation is
 *         impossible.
 */
void *
KIncrementalMalloc(const size_t size);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_MEMORY_ALLOCATION_INCREMENTAL_H */

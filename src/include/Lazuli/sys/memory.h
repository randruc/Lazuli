/**
 * @file src/include/Lazuli/sys/memory.h
 *
 * Defines base types and operations for memory management.
 */

#ifndef LZ_SYS_MEMORY_H
#define LZ_SYS_MEMORY_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Defines the kind of allocation for an AllocationMap.
 */
typedef enum {
  ALLOC_UNDEFINED,  /** < Undefined   */
  ALLOC_INCREMENTAL /** < Incremental */
}AllocationType;

/**
 * Represents a map of the allocated memory regions and useful memory handlers
 * for a task or the kernel.
 */
typedef struct {
  /** Base address of the memory region */
  void *baseMem;

  /** Break position, points to the first location beyond the current end of
      the heap */
  void *brk;

  /** < End address of the memory region */
  void *endMem;

  /** < The type of allocation used for this map */
  AllocationType allocationType;
}AllocationMap;

/**
 * Allocate memory for kernel objects using incremental method.
 *
 * @param s The size in bytes to allocate.
 *
 * @return A pointer to the allocated region, or NULL if allocation is
 *         impossible.
 */
void *
KIncrementalMalloc(const size_t s);

/**
 * Copy bytes from one location to another in main memory.
 *
 * @param source A pointer to the source address in RAM.
 * @param destination A pointer to the destination address in RAM.
 * @param length The number of bytes to copy.
 */
void
MemoryCopy(const void * source, void * destination, const size_t length);

_EXTERN_C_DECL_END

#endif /* LZ_SYS_MEMORY_H */

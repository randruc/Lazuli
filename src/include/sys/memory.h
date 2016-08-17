/**
 * @file memory.h
 *
 * Defines base types and operations for memory management.
 */

#ifndef LZ_MEMORY_H
#define LZ_MEMORY_H

#include <sys/utils.h>
#include <sys/types.h>

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
  void *baseMem;                       /** < Base address of the memory
										     region                           */
  void *brk;                           /** < Break position, points to the
										     first location beyond the curent
											 end of the heap                  */
  void *endMem;                        /** < End address of the memory region */
  AllocationType allocationType;       /** < The type of allocation used for
										     this map                         */
}AllocationMap;

_EXTERN_C_DECL_END

/**
 * Allocate memory for kernel objects using incremental method.
 *
 * @param s The size in bytes to allocate.
 *
 * @return A pointer to the allocated region, or NULL if allocation is
 *         impossible.
 */
void *
KIncrementalMalloc(size_t s);

#endif /* LZ_MEMORY_H */

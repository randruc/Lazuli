/**
 * @file malloc.h
 *
 * Defines base types and base operations for memory allocation.
 */

#ifndef MALLOC_H
#define MALLOC_H

#include <sys/utils.h>
#include <sys/types.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents a free memory block.
 */
struct alloc_block {
  size_t size;              /** < Amount of free memory in the block */
  struct alloc_block *next; /** < Next free block in the list */
};

/**
 * Represents a map of the allocated memory regions and useful handlers
 * for a task or the kernel.
 */
struct allocation_map {
  void *base_mem;                      /** < Base address of the memory
										     region */
  void *brk;                           /** < Break position, points to the first
                                             location beyond the curent end of
                                             the heap */
  void *end_mem;                       /** < End address of the memory region */
  struct alloc_block base_alloc_block; /** < First alloc block */
  struct alloc_block *blocks_list;     /** < Pointer to the free blocks list */
};
  
/**
 * Allocate a memory block and get a pointer to it.
 *
 * @param size The size to allocate.
 *
 * @return A pointer the the allocated block or NULL if no memory can be
 *         allocated.
 */
void *malloc(unsigned int size);
  
_EXTERN_C_DECL_END

#endif /* MALLOC_H */

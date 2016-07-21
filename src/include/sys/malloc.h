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
typedef struct allocation_block {
  size_t size;                   /** < Amount of free memory in the block */
  struct allocation_block *next; /** < Next free block in the list        */
}AllocationBlock;

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
  AllocationBlock baseAllocationBlock; /** < First alloc block                */
  AllocationBlock *blocksList;         /** < Pointer to the free blocks list  */
}AllocationMap;
  
/**
 * Allocate a memory block and get a pointer to it.
 * This implementation serves as a base for any task or for the kernel.
 *
 * @param size The size to allocate.
 * @param map A pointer to the concerned allocation map.
 *
 * @return A pointer the the allocated block or NULL if no memory can be
 *         allocated.
 */
void *
BaseMalloc(unsigned int size, AllocationMap *map);

/**
 * Free an allocated memory block.
 * This implementation serves as a base for any task or for the kernel.
 *
 * @param p A pointer to the block to free.
 * @param map A pointer to the concerned allocation map.
 */
void
BaseFree(void *p, AllocationMap *map);
  
_EXTERN_C_DECL_END

#endif /* MALLOC_H */

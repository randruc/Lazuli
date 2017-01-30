/**
 * @file malloc.c
 *
 * Implementation of dynamic memory allocation.
 * This can serve the kernel or the tasks.
 */

#include <sys/memory.h>
#include <sys/config.h>
#include <sys/types.h>
#include <sys/arch/AVR/arch.h>
#include <sys/kernel.h>

/**
 * Set break position of a memory region.
 *
 * @param increment The number of bytes to increment the break.
 * @param map A pointer to the concerned allocation map.
 *
 * @return A pointer to the beginning of the added memory block,
 *         or NULL if can't set break,
 *         or the current break position if increment is zero.
 */
static void *
SetBreak(const unsigned int increment, AllocationMap * const map)
{
  ptrdiff_t newGap;
  void *sp, *newBreak;
  
  if (map == NULL) {
    return NULL;
  }

  if (increment == 0) {
    return map->brk;
  }
  
  newBreak = ALLOW_ARITHM(map->brk) + increment;
  sp = GetStackPointer();
  newGap = ALLOW_ARITHM(sp) - ALLOW_ARITHM(newBreak);
  
  if (newGap < BREAK_STACK_GAP) {
    return NULL;
  }

  map->brk = ALLOW_ARITHM(map->brk) + increment;

  return ALLOW_ARITHM(map->brk) - increment;
}

/**
 * Common implementation of incremental memory allocation.
 *
 * @param s The size in byte to allocate.
 * @param map The allocation map of the task or kernel.
 *
 * @return A pointer to the allocated region, or NULL if allocation is
 *         impossible.
 */
static void *
BaseIncrementalMalloc(const size_t s, AllocationMap * const map)
{
  if (map == NULL) {
    return NULL;
  }
  
  if (map->allocationType == ALLOC_UNDEFINED) {
    map->allocationType = ALLOC_INCREMENTAL;
  } else if (map->allocationType != ALLOC_INCREMENTAL) {
    return NULL;
  }
  
  return SetBreak(s, map);
}

void *
KIncrementalMalloc(const size_t s)
{
  return BaseIncrementalMalloc(s, &kernelAllocationMap);
}

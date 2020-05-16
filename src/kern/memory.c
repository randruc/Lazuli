/**
 * @file src/kern/memory.c
 * @brief Kernel memory functions.
 * @date Jul 2016
 * @author Remi Andruccioli
 *
 * Memory functions for the kernel: allocation, copy, ...
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>

#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/memory.h>

/**
 * Set break position of a memory region.
 *
 * @param increment The number of bytes to increment the break.
 * @param map A pointer to the concerned allocation map.
 *
 * @return A pointer to the beginning of the allocated memory block,
 *         or NULL if can't set break,
 *         or the current break position if increment is zero.
 */
static void *
SetBreak(const unsigned int increment, AllocationMap * const map)
{
  ptrdiff_t newGap;
  void *newBreak;
  void *oldBreak;

  if (NULL == map) {
    return NULL;
  }

  if (0 == increment) {
    return map->brk;
  }

  /* TODO: Check if this one is a good idea... */
  oldBreak = map->brk;
  newBreak = ALLOW_ARITHM(oldBreak) + increment;
  newGap = ALLOW_ARITHM(SP) - ALLOW_ARITHM(newBreak);

  if (newGap < LZ_CONFIG_BREAK_STACK_GAP) {
    return NULL;
  }

  map->brk = newBreak;

  return oldBreak;
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
  if (NULL == map) {
    return NULL;
  }

  /* We force the type of allocation to be incremental by default */
  if (ALLOC_UNDEFINED == map->allocationType) {
    map->allocationType = ALLOC_INCREMENTAL;
  } else if (ALLOC_INCREMENTAL != map->allocationType) {
    return NULL;
  }

  return SetBreak(s, map);
}

void *
KIncrementalMalloc(const size_t size)
{
  return BaseIncrementalMalloc(size, &kernelAllocationMap);
}

void
Memory_Copy(const void *source, void *destination, const size_t size)
{
  size_t i;
  const uint8_t *sourceBytes = source;
  uint8_t *destinationBytes = destination;

  for (i = 0; i < size; ++i) {
    destinationBytes[i] = sourceBytes[i];
  }
}

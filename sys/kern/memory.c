/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Kernel memory functions.
 * @copyright 2016-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
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
  /* TODO: Check for overflows */
  oldBreak = map->brk;
  newBreak = ALLOW_ARITHM(oldBreak) + increment;
  newGap = ALLOW_ARITHM(SP) - ALLOW_ARITHM(newBreak);

  if (newGap < LZ_CONFIG_BREAK_STACK_GAP) {
    return NULL;
  }

  map->brk = newBreak;

  return oldBreak;
}

/*
 * For now, only incremental memory allocation is supported.
 */
void *
KIncrementalMalloc(const size_t size)
{
  return SetBreak(size, &kernelAllocationMap);
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

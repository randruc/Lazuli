/**
 * @file src/kern/memory.c
 * @brief Kernel memory functions.
 * @date Jul 2016
 * @author Remi Andruccioli
 *
 * Common memory functions for the kernel.
 */
/* TODO: Maybe this file is useless and Memory_Copy should be in stdlib... */

#include <stdint.h>

#include <Lazuli/common.h>

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

/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Memory management API.
 * @copyright 2017-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * Defines base types and operations for memory management.
 */

#ifndef LAZULI_SYS_MEMORY_H
#define LAZULI_SYS_MEMORY_H

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
 * Allocate memory for kernel objects using incremental method.
 *
 * @param size The size in bytes to allocate.
 *
 * @return A pointer to the allocated region, or NULL if allocation is
 *         impossible.
 */
void *
KIncrementalMalloc(const size_t size);

/**
 * Copy bytes from one location to another in main memory.
 *
 * @param source A pointer to the source address in RAM.
 * @param destination A pointer to the destination address in RAM.
 * @param size The number of bytes to copy.
 */
void
Memory_Copy(const void * source, void * destination, const size_t size);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_MEMORY_H */

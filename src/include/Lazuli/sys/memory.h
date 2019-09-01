/**
 * @file src/include/Lazuli/sys/memory.h
 *
 * Defines base types and operations for memory management.
 */

#ifndef LAZULI_SYS_MEMORY_H
#define LAZULI_SYS_MEMORY_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

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

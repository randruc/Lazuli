/**
 * @file src/include/Lazuli/sys/kernel.h
 *
 * Kernel symbols definition.
 */

#ifndef LZ_SYS_KERNEL_H
#define LZ_SYS_KERNEL_H

#include <Lazuli/sys/memory.h>
#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Allocation map for the kernel.
 */
extern AllocationMap kernelAllocationMap;

/**
 * Kernel panic.
 */
void
Panic();

/**
 * Reverse the bytes of a 16-bit (2-byte) word.
 *
 * @warning Assuming that the parameter value points to a valid 16-bit word.
 *
 * @param value A universal pointer to a 16-bit word to reverse.
 */
void
ReverseBytes16(void * const value);

_EXTERN_C_DECL_END

#endif /* LZ_SYS_KERNEL_H */

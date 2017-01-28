/**
 * @file kernel.h
 *
 * Defines kernel objects.
 */

#ifndef LZ_KERNEL_H
#define LZ_KERNEL_H

#include <sys/memory.h>
#include <sys/types.h>

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

_EXTERN_C_DECL_END

#endif /* LZ_KERNEL_H */

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

_EXTERN_C_DECL_END

#endif /* LZ_SYS_KERNEL_H */

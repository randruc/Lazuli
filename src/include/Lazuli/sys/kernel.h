/**
 * @file src/include/Lazuli/sys/kernel.h
 *
 * Kernel symbols definition.
 */

#ifndef LAZULI_SYS_KERNEL_H
#define LAZULI_SYS_KERNEL_H

#include <Lazuli/common.h>

#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/memory.h>

_EXTERN_C_DECL_BEGIN

/**
 * Allocation map for the kernel.
 */
extern AllocationMap kernelAllocationMap;

/**
 * Kernel panic.
 *
 * @warning This function is declared to never return!
 *          If this has to evolve, remember to remove __noreturn declaration on
 *          this function.
 */
__noreturn void
Panic(void);

/**
 * Reverse the bytes of a function pointer.
 *
 * @param pointer A function pointer.
 *
 * @return A new function pointer from reversed bytes of parameter "pointer".
 */
void
(*ReverseBytesOfFunctionPointer(void (* const pointer)(void)))(void);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_KERNEL_H */

/**
 * @file src/include/Lazuli/sys/kernel.h
 *
 * Kernel symbols definition.
 */

#ifndef LAZULI_SYS_KERNEL_H
#define LAZULI_SYS_KERNEL_H

#include <Lazuli/common.h>

#include <Lazuli/sys/compiler.h>

_EXTERN_C_DECL_BEGIN

/**
 * Kernel panic.
 *
 * @warning This function is declared to never return!
 *          If this has to evolve, remember to remove NORETURN declaration on
 *          this function.
 */
NORETURN void
Kernel_Panic(void);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_KERNEL_H */

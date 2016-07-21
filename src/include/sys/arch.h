/**
 * @file arch.h
 *
 * Defines architecture specific functions for AVR.
 */

#ifndef ARCH_H
#define ARCH_H

#include <sys/utils.h>

_EXTERN_C_DECL_BEGIN

/**
 * Get the current stack pointer.
 *
 * @return The address pointed by the machine stack pointer.
 */
void *GetStackPointer();

_EXTERN_C_DECL_END

#endif /* ARCH_H */

/**
 * @file src/include/Lazuli/sys/compiler.h
 * @brief Macro aliases on compiler facilities.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file defines macros that can be used as aliases on compiler facilities
 * to add non-standard features to pure C89.
 * Ideally kernel code should work without these macros. These are just the
 * icing on the cake.
 */

#ifndef LZ_SYS_COMPILER_H
#define LZ_SYS_COMPILER_H

#ifdef __GNUC__

/**
 * Indicates that a function never returns.
 */
#define __noreturn __attribute__((noreturn))

/**
 * Declare a variable to be stored in program memory.
 * i.e. usualy in ROM.
 */
#define __progmem __attribute__((section(".progmem")))

#else /* __GNUC__ */

#define __noreturn
#define __progmem

#endif/* __GNUC__ */

#endif /* LZ_SYS_COMPILER_H */

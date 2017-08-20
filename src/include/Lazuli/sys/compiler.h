/**
 * @file src/include/Lazuli/sys/compiler.h
 *
 * Macro aliases on compiler facilities.
 */

#ifndef LZ_SYS_COMPILER_H
#define LZ_SYS_COMPILER_H

#ifdef __GNUC__

/**
 * Indicates that a function never returns.
 */
#define noreturn __attribute__((noreturn))

/**
 * Declare a variable to be stored in program memory.
 * i.e. usualy in ROM.
 */
#define progmem __attribute__((section(".progmem")))

#else /* __GNUC__ */

#define noreturn
#define progmem

#endif/* __GNUC__ */

#endif /* LZ_SYS_COMPILER_H */

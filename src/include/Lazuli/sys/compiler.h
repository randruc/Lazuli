/**
 * @file compiler.h
 *
 * Macro aliases on compiler facilities.
 */

#ifndef LZ_COMPILER_H
#define LZ_COMPILER_H

#ifdef __GNUC__

/**
 * Indicates that a function never returns.
 */
#define noreturn __attribute__((noreturn))

#else /* __GNUC__ */

#define noreturn

#endif/* __GNUC__ */

#endif /* LZ_COMPILER_H */

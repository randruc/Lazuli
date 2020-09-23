/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief AVR type and macros definitions for variadic functions.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file defines types and macro to support variadic functions on the AVR
 * architecture.
 */

#ifndef STDARG_H
#define STDARG_H

#ifdef __GNUC__

/*
 * This implementation relies on GCC builtins. This is contradictory with the
 * goal of implementing all of this project in pure C. However, the other
 * implementation shown in this file, which is pure C, suffers from severe
 * limitations. So for now we prefer to keep using this implementation as it
 * imposes less constraints to the final user.
 */

/**
 * Represents the object holding informations needed to access variadic function
 * parameters.
 */
typedef __builtin_va_list va_list;

/**
 * Initializes the access to variadic function parameters.
 *
 * @param ap An instance of va_list.
 * @param last The last named parameter.
 */
#define va_start(ap, last) __builtin_va_start(ap, last)

/**
 * Obtain the next parameter of the variadic function.
 *
 * @param ap An instance of va_list.
 * @param type The type of the expected parameter.
 *             Due to type promotion, specifying a larger type than the desired
 *             type might be required. e.g. uint8_t might be promoted to int, so
 *             int must be specified instead of uint8_t.
 */
#define va_arg(ap, type) __builtin_va_arg(ap, type)

/**
 * Ends the access to variadic function parameters.
 *
 * @param ap An instance of va_list.
 */
#define va_end(ap) __builtin_va_end(ap)

/**
 * Copy a va_list object.
 *
 * @param dest The destination instance of va_list
 * @param src The source instance of va_list.
 *
 * @warning This function must be called before any call to va_arg.
 */
#define va_copy(dest, src) __builtin_va_copy(dest, src)

#if 0

/*
 * Here we introduce another possible implementation: thanks to the AVR GCC
 * calling convention, we can implement access to parameters of variadic
 * functions in pure C.
 * However, be aware that this implementation has the following flaws:
 * - It doesn't work with structures passed by value (read below).
 * - It doesn't work when GCC chooses to inline a function.
 *
 * So if you really want to use this implementation, make sure to:
 * - Never pass a structure by value to a function.
 * - Use the appropriate compiler attribute to tell the compiler to NEVER inline
 *   the variadic function.
 *
 * The conclusion for now is that any of the two implementation in this file
 * require the use of compiler specifics/builtins (unfortunately).
 * We choose to show this implementation here as a starting point for further
 * investigation, if needed.
 *
 * For the AVR caling convention on GCC, see:
 * https://gcc.gnu.org/wiki/avr-gcc
 */

#include <stdint.h>

/**
 * Represents the object holding informations needed to access variadic function
 * parameters.
 */
/* We choose uint8_t here, but it works with any type of size == 1 */
typedef uint8_t* va_list;

/**
 * Initializes the access to variadic function parameters.
 *
 * @param ap An instance of va_list.
 * @param last The last named parameter.
 */
#define va_start(ap, last)                      \
  (ap) = (va_list)((&(last)) + 1)

/**
 * Obtain the next parameter of the variadic function.
 *
 * @param ap An instance of va_list.
 * @param type The type of the expected parameter.
 *
 * @warning Getting the value of a structure parameter passed by value is not
 *          supported.
 */
/*
 * Again, this is very specific to the AVR GCC calling convention.
 *
 * It works because in C the assignment operator "=" have higher precedence than
 * the comma "," operator. i.e. In this definition the part before the comma is
 * part of an assignment when the macro is called, so the assignment will be
 * evaluated first, then everything that is after the comma is executed.
 *
 * One note: it seems that passing structure by value is not supported because
 * AVR GCC doesn't follow the usual ABI rules for pushing these types on the
 * stack. For example, trying with a structure of size 3, the compiler will push
 * every byte of it without padding to round up its size to the next even value.
 * Even structures of size 1 seem to have the same behaviour.
 */
#define va_arg(ap, type)                                                \
  /* First, we dereference to get the value                          */ \
  (*(type*)ap),                                                         \
    /* Then we need to point after the object we just get,           */ \
    /* So if its size is an odd number of bytes                      */ \
    ((sizeof(type) & 1) ?                                               \
     /* We round it up to the next even size before pointing         */ \
     /* to the next value                                            */ \
     (ap += sizeof(type) + 1) :                                         \
     /* Else we point directly after the value                       */ \
     (ap += sizeof(type)))

/**
 * Ends the access to variadic function parameters.
 *
 * @param ap An instance of va_list.
 */
#define va_end(ap)                              \
  do {} while (0)

/**
 * Copy a va_list object.
 *
 * @param dest The destination instance of va_list
 * @param src The source instance of va_list.
 *
 * @warning This function must be called before any call to va_arg.
 */
#define va_copy(dest, src)                      \
  dest = src

#endif /* 0 */

#else /* __GNUC__ */
#error "stdarg.h is not implemented for this compiler."
#endif

#endif /* STDARG_H */

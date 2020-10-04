/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Basic type definitions and useful macros.
 * @copyright 2017-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes basic type definitions and useful macros used in the
 * Lazuli project.
 */

#ifndef LAZULI_COMMON_H
#define LAZULI_COMMON_H

#include <stdint.h>

#ifdef __cplusplus

/**
 * Open C++ header file declarations.
 */
#define _EXTERN_C_DECL_BEGIN extern "C" {

/**
 * Close C++ header file declarations.
 */
#define _EXTERN_C_DECL_END }

#else /* __cplusplus */

/**
 * Open C++ header file declarations.
 */
#define _EXTERN_C_DECL_BEGIN

/**
 * Close C++ header file declarations.
 */
#define _EXTERN_C_DECL_END

#endif /* __cplusplus */

/**
 * Tell the compiler that the variable X is left unsed.
 *
 * @param X The unused variable.
 */
#define UNUSED(X) ((void)(X))

/**
 * Perform an assertion at compile time.
 * Won't compile if the assertion is false.
 *
 * @param C The condition to check.
 * @param M A C-language identifier used as "message/name" for the static
 *          assertion.
 *          It's used here to display an informative message directly in the
 *          source code and compiler output in case of a failure.
 */
#define STATIC_ASSERT(C, M) extern char _static_assertion_##M[(C) ? 1 : -1]

/**
 * NULL pointer.
 */
#define NULL ((void *)0)

/**
 * Represents the difference between two pointers.
 */
typedef int ptrdiff_t;

/**
 * Boolean type.
 */
typedef uint8_t bool;

/** @cond false */
STATIC_ASSERT(sizeof(bool) == 1, bool_MUST_be_1_byte_long);
/** @endcond */

/**
 * Boolean TRUE constant value.
 */
#define true ((bool)1)

/**
 * Boolean FALSE constant value.
 */
#define false ((bool)0)

/* TODO: Not portable!!! */
/**
 * Represents the size of an object.
 */
typedef uint16_t size_t;

/* TODO: Arch specific */
/**
 * Represents an unsigned integer that can be read and written atomically.
 * This type is the equivalent of libc's sig_atomic_t (signal.h).
 *
 * Use this type with volatile.
 */
typedef uint8_t u_read_write_atomic_t;

/**
 * @brief Allow arithmetic on a void pointer.
 *
 * Arithmetic on void pointers isn't allowed in standard C.
 *
 * @param X The void pointer on which to perform arithmetic.
 */
#define ALLOW_ARITHM(X) ((uint8_t *)(X))

/**
 * @cond false
 *
 * This assertion is mandatory for ALLOW_ARITHM macro.
 * ALLOW_ARITHM can work with any type whose size is 1 byte.
 */
STATIC_ASSERT(sizeof(uint8_t) == 1,
              uint8_t_MUST_be_1_byte_long_to_allow_arithmetic_on_void_pointers);
/** @endcond */

/**
 * Check that the lvalue V is of type T at compile time.
 *
 * @param V The lvalue to check.
 * @param T The supposed type of the lvalue.
 */
#define STATIC_CHECK_TYPE(V, T) UNUSED(1 ? (T*)0 : &(V))

/**
 * Set the corresponding bits of X in the value V of type T.
 *
 * @param V The value to set bits.
 * @param T The type of the value V.
 * @param X A value containing the bits (at logical 1) to set.
 */
#define SET_BITS(V, T, X)                           \
  do {                                              \
    STATIC_CHECK_TYPE(V, T);                        \
    (V) |= (T)(X);                                  \
  } while (0)

/**
 * Clear the corresponding bits of X in the value V of type T.
 *
 * @param V The value to set bits.
 * @param T The type of the value V.
 * @param X A value containing the bits (at logical 1) to clear.
 */
#define CLEAR_BITS(V, T, X)                     \
  do {                                          \
    STATIC_CHECK_TYPE(V, T);                    \
    (V) &= (T)(~(X));                           \
  } while (0)

/**
 * Define a constant bit at position X, starting from index 0.
 *
 * @param X An unsigned integer constant representing the position of the bit,
 *          starting from index 0.
 *
 * @warning The constant must be specified using an unsigned integer litteral
 *          in uppercase.
 *          e.g. POSITION(2U)
 *          This is to make static analyzers not complain about using a shift
 *          operator with an unsigned value.
 */
#define POSITION(X) (1U << (X))

/**
 * Define an indirect read/write variable at an absolute address.
 * i.e. A variable that is accessed through a pointer.
 *
 * @param X The address of the variable.
 * @param T The type of the variable.
 */
#define INDIRECT_T(X, T) ((volatile T *)(X))

/**
 * Define an indirect read/write register at an absolute address.
 * i.e. A register that is accessed through a pointer.
 *
 * @param X The address of the register.
 */
#define INDIRECT(X) INDIRECT_T(X, uint8_t)

/**
 * Define a direct read/write variable at an absolute address.
 *
 * @param X The address of the variable.
 * @param T The type of the variable.
 */
#define DIRECT_T(X, T) (*INDIRECT_T(X, T))

/**
 * Define a direct read/write register at an absolute address.
 *
 * @param X The address of the register.
 */
#define DIRECT(X) DIRECT_T(X, uint8_t)

/**
 * Take the low byte of a 16-bit value.
 *
 * @param X The 16-bit value.
 */
#define LO8(X) ((uint8_t)(X))

/**
 * Take the high byte of a 16-bit value.
 *
 * @param X The 16-bit value.
 */
#define HI8(X) LO8((X) >> 8U)

/**
 * Obtain the minimum between two integer values.
 *
 * @param A The first value.
 * @param B The second value.
 */
#define MIN(A, B) (((A) < (B)) ? (A) : (B))

/**
 * Obtain the maximum between two integer values.
 *
 * @param A The first value.
 * @param B The second value.
 */
#define MAX(A, B) (((A) < (B)) ? (B) : (A))

/**
 * Obtain the absolute value of an integer.
 *
 * @param A The integer value.
 */
#define ABS(A) (((A) < 0) ? (-(A)) : (A))

/**
 * Get the offset of a member in a structure.
 *
 * @param M The name of the member.
 * @param T The type of the structure.
 */
#define OFFSET_OF(M, T)                         \
  ((size_t)(&(((T*)0)->M)))

/**
 * Get a pointer to the structure T containing the member M pointed by P.
 *
 * May not compile if P doesn't point to the type defined by M.
 *
 * @param P The pointer to the member.
 * @param M The name of the member.
 * @param T The type of the structure.
 */
#define CONTAINER_OF(P, M, T)                                   \
  ((T*) ((uint8_t*) (1 ? (P) : &(((T*)0)->M)) - OFFSET_OF(M, T)))

/**
 * Convert the name of a symbol to a string.
 *
 * @param X The symbol name to convert.
 */
#define NAME_OF(X) #X

/**
 * Get the number of elements in a statically initialized array.
 *
 * @param X The array variable.
 */
#define ELEMENTS_COUNT(X) ((size_t)(sizeof(X) / sizeof((X)[0])))

/**
 * Perform a static assertion to check if a given module is activated.
 *
 * @param X The module name, in uppercase.
 */
#define DEPENDENCY_ON_MODULE(X) \
  STATIC_ASSERT(LZ_CONFIG_MODULE_ ## X ## _USED, Module_ ## X ## _must_be_used)

#endif /* LAZULI_COMMON_H */

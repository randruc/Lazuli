/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Fixed width integer types for x86_64.
 * @copyright 2018-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the implementation of the stdint.h standard header for
 * defining fixed width integer types for the x86_64 architecture.
 *
 * This implementation is architecture-dependent.
 *
 * This implementation is based on :
 * en.cppreference.com/w/c/types/integer
 * pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdint.h.html
 */

#ifndef STDINT_H
#define STDINT_H

/**
 * Represents a signed integer type with width of exactly 8 bits.
 */
typedef char int8_t;

/**
 * Represents a signed integer type with width of exactly 16 bits.
 */
typedef short int16_t;

/**
 * Represents a signed integer type with width of exactly 32 bits.
 */
typedef int int32_t;

/**
 * Represents a signed integer type with width of exactly 64 bits.
 */
typedef long int64_t;

/**
 * Represents the fastest signed integer type with width of at least 8 bits.
 */
typedef char int_fast8_t;

/**
 * Represents the fastest signed integer type with width of at least 16 bits.
 */
typedef short int_fast16_t;

/**
 * Represents the fastest signed integer type with width of at least 32 bits.
 */
typedef int int_fast32_t;

/**
 * Represents the fastest signed integer type with width of at least 64 bits.
 */
typedef long int_fast64_t;

/**
 * Represents the smallest signed integer type with width of at least 8 bits.
 */
typedef unsigned char int_least8_t;

/**
 * Represents the smallest signed integer type with width of at least 16 bits.
 */
typedef unsigned short int_least16_t;

/**
 * Represents the smallest signed integer type with width of at least 32 bits.
 */
typedef unsigned int int_least32_t;

/**
 * Represents the smallest signed integer type with width of at least 64 bits.
 */
typedef unsigned long int_least64_t;

/**
 * Represents the maximum width integer type.
 *
 * Represents a signed integer type capable of representing any value of any
 * signed integer type.
 */
typedef int64_t intmax_t;

/**
 * Represents an integer capable of holding a pointer.
 *
 * Represents a signed integer type with the property that any valid pointer to
 * void can be converted to this type, then converted back to a pointer to void,
 * and the result will compare equal to the original pointer.
 */
typedef int64_t intptr_t;

/**
 * Represents a unsigned integer type with width of exactly 8 bits.
 */
typedef unsigned char uint8_t;

/**
 * Represents a unsigned integer type with width of exactly 16 bits.
 */
typedef unsigned short uint16_t;

/**
 * Represents a unsigned integer type with width of exactly 32 bits.
 */
typedef unsigned int uint32_t;

/**
 * Represents a unsigned integer type with width of exactly 64 bits.
 */
typedef unsigned long uint64_t;

/**
 * Represents the fastest unsigned integer type with width of at least 8 bits.
 */
typedef unsigned char uint_fast8_t;

/**
 * Represents the fastest unsigned integer type with width of at least 16 bits.
 */
typedef unsigned short uint_fast16_t;

/**
 * Represents the fastest unsigned integer type with width of at least 32 bits.
 */
typedef unsigned int uint_fast32_t;

/**
 * Represents the fastest unsigned integer type with width of at least 64 bits.
 */
typedef unsigned long uint_fast64_t;

/**
 * Represents the smallest unsigned integer type with width of at least 8 bits.
 */
typedef unsigned char uint_least8_t;

/**
 * Represents the smallest unsigned integer type with width of at least 16 bits.
 */
typedef unsigned short uint_least16_t;

/**
 * Represents the smallest unsigned integer type with width of at least 32 bits.
 */
typedef unsigned int uint_least32_t;

/**
 * Represents the smallest unsigned integer type with width of at least 64 bits.
 */
typedef unsigned long uint_least64_t;

/**
 * Represents the maximum width unsigned integer type.
 *
 * Represents an unsigned integer type capable of representing any value of any
 * unsigned integer type.
 */
typedef uint64_t uintmax_t;

/**
 * Represents an unsigned integer capable of holding a pointer.
 *
 * Represents an unsigned integer type with the property that any valid pointer
 * to void can be converted to this type, then converted back to a pointer to
 * void, and the result will compare equal to the original pointer.
 */
typedef uint64_t uintptr_t;

/**
 * Represents the minimum value of an object of type int8_t.
 */
#define INT8_MIN ((int8_t)0x80)

/**
 * Represents the minimum value of an object of type int16_t.
 */
#define INT16_MIN ((int16_t)0x8000)

/**
 * Represents the minimum value of an object of type int32_t.
 */
#define INT32_MIN ((int32_t)0x80000000)

/**
 * Represents the minimum value of an object of type int64_t.
 */
#define INT64_MIN ((int64_t)0x8000000000000000)

/**
 * Represents the minimum value of an object of type int_fast8_t.
 */
#define INT_FAST8_MIN INT8_MIN

/**
 * Represents the minimum value of an object of type int_fast16_t.
 */
#define INT_FAST16_MIN INT16_MIN

/**
 * Represents the minimum value of an object of type int_fast32_t.
 */
#define INT_FAST32_MIN INT32_MIN

/**
 * Represents the minimum value of an object of type int_fast64_t.
 */
#define INT_FAST64_MIN INT64_MIN

/**
 * Represents the minimum value of an object of type int_least8_t.
 */
#define INT_LEAST8_MIN INT8_MIN

/**
 * Represents the minimum value of an object of type int_least16_t.
 */
#define INT_LEAST16_MIN INT16_MIN

/**
 * Represents the minimum value of an object of type int_least32_t.
 */
#define INT_LEAST32_MIN INT32_MIN

/**
 * Represents the minimum value of an object of type int_least64_t.
 */
#define INT_LEAST64_MIN INT64_MIN

/**
 * Represents the minimum value of an object of type intptr_t.
 */
#define INTPTR_MIN INT64_MIN

/**
 * Represents the maximum value of an object of type int8_t.
 */
#define INT8_MAX ((int8_t)0x7f)

/**
 * Represents the maximum value of an object of type int16_t.
 */
#define INT16_MAX ((int16_t)0x7fff)

/**
 * Represents the maximum value of an object of type int32_t.
 */
#define INT32_MAX ((int32_t)0x7fffffff)

/**
 * Represents the maximum value of an object of type int64_t.
 */
#define INT64_MAX ((int64_t)0x7fffffffffffffff)

/**
 * Represents the maximum value of an object of type int_fast8_t.
 */
#define INT_FAST8_MAX INT8_MAX

/**
 * Represents the maximum value of an object of type int_fast16_t.
 */
#define INT_FAST16_MAX INT16_MAX

/**
 * Represents the maximum value of an object of type int_fast32_t.
 */
#define INT_FAST32_MAX INT32_MAX

/**
 * Represents the maximum value of an object of type int_fast64_t.
 */
#define INT_FAST64_MAX INT64_MAX

/**
 * Represents the maximum value of an object of type int_least8_t.
 */
#define INT_LEAST8_MAX INT8_MAX

/**
 * Represents the maximum value of an object of type int_least16_t.
 */
#define INT_LEAST16_MAX INT16_MAX

/**
 * Represents the maximum value of an object of type int_least32_t.
 */
#define INT_LEAST32_MAX INT32_MAX

/**
 * Represents the maximum value of an object of type int_least64_t.
 */
#define INT_LEAST64_MAX INT64_MAX

/**
 * Represents the maximum value of an object of type intptr_t.
 */
#define INTPTR_MAX INT64_MAX

/**
 * Represents the maximum value of an object of type uint8_t.
 */
#define UINT8_MAX ((uint8_t)0xff)

/**
 * Represents the maximum value of an object of type uint16_t.
 */
#define UINT16_MAX ((uint16_t)0xffff)

/**
 * Represents the maximum value of an object of type uint32_t.
 */
#define UINT32_MAX ((uint32_t)0xffffffff)

/**
 * Represents the maximum value of an object of type uint64_t.
 */
#define UINT64_MAX ((uint64_t)0xffffffffffffffff)

/**
 * Represents the maximum value of an object of type uint_fast8_t.
 */
#define UINT_FAST8_MAX UINT8_MAX

/**
 * Represents the maximum value of an object of type uint_fast16_t.
 */
#define UINT_FAST16_MAX UINT16_MAX

/**
 * Represents the maximum value of an object of type uint_fast32_t.
 */
#define UINT_FAST32_MAX UINT32_MAX

/**
 * Represents the maximum value of an object of type uint_fast64_t.
 */
#define UINT_FAST64_MAX UINT64_MAX

/**
 * Represents the maximum value of an object of type uint_least8_t.
 */
#define UINT_LEAST8_MAX UINT8_MAX

/**
 * Represents the maximum value of an object of type uint_least16_t.
 */
#define UINT_LEAST16_MAX UINT16_MAX

/**
 * Represents the maximum value of an object of type uint_least32_t.
 */
#define UINT_LEAST32_MAX UINT32_MAX

/**
 * Represents the maximum value of an object of type uint_least64_t.
 */
#define UINT_LEAST64_MAX UINT64_MAX

/**
 * Represents the maximum value of an object of type uinptr_t.
 */
#define UINTPTR_MAX UINT64_MAX

/**
 * Represents the maximum value of an object of type uintmax_t.
 */
#define UINTMAX_MAX UINT64_MAX

/**
 * Expands to an integer constant expression having the value specified by its
 * argument and the type int_least8_t.
 */
#define INT8_C(X) ((int_least8_t)(X))

/**
 * Expands to an integer constant expression having the value specified by its
 * argument and the type int_least16_t.
 */
#define INT16_C(X) ((int_least16_t)(X))

/**
 * Expands to an integer constant expression having the value specified by its
 * argument and the type int_least32_t.
 */
#define INT32_C(X) ((int_least32_t)(X))

/**
 * Expands to an integer constant expression having the value specified by its
 * argument and the type int_least64_t.
 */
#define INT64_C(X) ((int_least64_t)(X))

/**
 * Expands to an integer constant expression having the value specified by its
 * argument and the type intmax_t.
 */
#define INTMAX_C(X) ((intmax_t)(X))

/**
 * Expands to an integer constant expression having the value specified by its
 * argument and the type uint_least8_t.
 */
#define UINT8_C(X) ((uint_least8_t)(X))

/**
 * Expands to an integer constant expression having the value specified by its
 * argument and the type uint_least16_t.
 */
#define UINT16_C(X) ((uint_least16_t)(X))

/**
 * Expands to an integer constant expression having the value specified by its
 * argument and the type uint_least32_t.
 */
#define UINT32_C(X) ((uint_least32_t)(X))

/**
 * Expands to an integer constant expression having the value specified by its
 * argument and the type uint_least64_t.
 */
#define UINT64_C(X) ((uint_least64_t)(X))

/**
 * Expands to an integer constant expression having the value specified by its
 * argument and the type uintmax_t.
 */
#define UINTMAX_C(X) ((uintmax_t)(X))

/* Format macro constants not defined */

#endif /* STDINT_H */

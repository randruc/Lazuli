/**
 * @file types.h
 *
 * This file contains useful macros and describes basic types used in the
 * kernel.
 */
/* TODO: Maybe consider renaming this file */

#ifndef LZ_TYPES_H
#define LZ_TYPES_H

#ifdef __cplusplus
/** 
 * Open C++ header file declarations
 */    
#define _EXTERN_C_DECL_BEGIN extern "C" {
/** 
 * Close C++ header file declarations
 */    
#define _EXTERN_C_DECL_END }
#else /* __cplusplus */
#define _EXTERN_C_DECL_BEGIN
#define _EXTERN_C_DECL_END
#endif /* __cplusplus */

/**
 * Tell the compiler that the value X is left unsed.
 *
 * @param X The unused value.
 */
#define UNUSED(X) ((void)(X))

/**
 * Allow arithmetic on a void pointer.
 * Arithmetic on void pointers isn't allowed in standard C.
 *
 * @param X The void pointer on which to perform arithmetic.
 */
#define ALLOW_ARITHM(X) ((u8 *)X)

/**
 * Perform an assertion at compile time.
 * Won't compile if the assertion is false.
 *
 * @param C The condition to check.
 * @param M A string explaining why the assertion is performed. This parameter
 *          is not actualy used, it's only here to display an informative
 *          message directly in the source code in case of a failure.
 */
#define STATIC_ASSERT(C, M) extern u8 _static_assertion[(C) ? 1 : -1]

/**
 * Check that the lvalue V is of type T at compile time.
 *
 * @param V The lvalue to check.
 * @param T The supposed type of the lvalue.
 */
#define STATIC_CHECK_TYPE(V, T) UNUSED(1 ? &V : (T*)0)

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
    V |= (T)(X);                                    \
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
    V &= (T)(~(X));                             \
  } while (0)

/**
 * Define a constant bit at position X.
 *
 * @param X An integer representing the position of the bit.
 */
#define POSITION(X) (1 << (X))

/**
 * Define an indirect read/write register at an aboslute address.
 * i.e. A register that is accessed through a pointer.
 *
 * @param X The address of the register.
 */
#define INDIRECT(X) ((volatile u8 *)(X))

/**
 * Define a direct read/write register at an aboslute address.
 *
 * @param X The address of the register.
 */
#define DIRECT(X) (*INDIRECT(X))

/**
 * NULL pointer.
 */
#define NULL ((void *)0)

/**
 * Represents the size of an object.
 */
typedef unsigned int size_t;

/**
 * Represents the difference between two pointers.
 */
typedef int ptrdiff_t;

/**
 * Unsigned integer of 1 byte long.
 */
typedef unsigned char u8;
STATIC_ASSERT(sizeof(u8) == 1, "u8 MUST be 1 byte long.");

/**
 * Signed integer of 1 byte long.
 */
typedef signed char s8;
STATIC_ASSERT(sizeof(s8) == 1, "s8 MUST be 1 byte long.");

/**
 * Unsigned integer of 2 bytes long.
 */
typedef unsigned int u16;
STATIC_ASSERT(sizeof(u16) == 2, "u16 MUST be 2 bytes long.");

/**
 * Signed integer of 2 bytes long.
 */
typedef signed int s16;
STATIC_ASSERT(sizeof(s16) == 2, "s16 MUST be 2 bytes long.");

/**
 * Unsigned integer of 4 bytes long.
 */
typedef unsigned long int u32;
STATIC_ASSERT(sizeof(u32) == 4, "u32 MUST be 4 bytes long.");

/**
 * Signed integer of 4 bytes long.
 */
typedef signed long int s32;
STATIC_ASSERT(sizeof(s32) == 4, "s32 MUST be 4 bytes long.");

/**
 * Boolean type.
 */
typedef u8 bool;
STATIC_ASSERT(sizeof(bool) == 1, "bool MUST be 1 byte long.");

/**
 * Maximum value of u8.
 */
#define u8_MAX ((u8)0xff)

/**
 * Minimum value of u8.
 */
#define u8_MIN ((u8)0x00)

/**
 * Maximum value of s8.
 */
#define s8_MAX ((s8)0x7f)

/**
 * Minimum value of s8.
 */
#define s8_MIN ((s8)0x80)

/**
 * Maximum value of u16.
 */
#define u16_MAX ((u16)0xffff)

/**
 * Minimum value of u16.
 */
#define u16_MIN ((u16)0x0000)

/**
 * Maximum value of s16.
 */
#define s16_MAX ((s16)0x7fff)

/**
 * Minimum value of s16.
 */
#define s16_MIN ((s16)0x8000)

/**
 * Maximum value of u32.
 */
#define u32_MAX ((u32)0xffffffff)

/**
 * Minimum value of u32.
 */
#define u32_MIN ((u32)0x00000000)

/**
 * Maximum value of s32.
 */
#define s32_MAX ((s32)0x7fffffff)

/**
 * Minimum value of s32.
 */
#define s32_MIN ((s32)0x80000000)

/**
 * Boolean TRUE constant value.
 */
#define true ((bool)1)

/**
 * Boolean FALSE constant value.
 */
#define false ((bool)0);

#endif /* LZ_TYPES_H */

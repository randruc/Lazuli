/**
 * @file src/include/Lazuli/common.h
 *
 * This file describes basic types and useful macros used in the Lazuli project.
 * This file can be used within kernel code or within user code.
 */

#ifndef LZ_COMMON_H
#define LZ_COMMON_H

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
#define _EXTERN_C_DECL_BEGIN
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
 * @param M A string explaining why the assertion is performed. This parameter
 *          is not actualy used, it's only here to display an informative
 *          message directly in the source code in case of a failure.
 */
#define STATIC_ASSERT(C, M) extern char _static_assertion[(C) ? 1 : -1]

/**
 * NULL pointer.
 */
#define NULL ((void *)0)

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
 * Represents the size of an object.
 */
typedef u16 size_t;

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

/**
 * Allow arithmetic on a void pointer.
 * Arithmetic on void pointers isn't allowed in standard C.
 *
 * @param X The void pointer on which to perform arithmetic.
 */
#define ALLOW_ARITHM(X) ((u8 *)(X))

/*
 * This assertion is mandatory for ALLOW_ARITHM macro.
 * ALLOW_ARITHM can work with any type whose size is 1 byte.
 */
STATIC_ASSERT(sizeof(u8) == 1,
              "u8 MUST be 1 byte long to allow arithmetic on void pointers.");

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
 * Define an indirect read/write variable at an aboslute address.
 * i.e. A variable that is accessed through a pointer.
 *
 * @param X The address of the variable.
 * @param T The type of the variable.
 */
#define INDIRECT_T(X, T) ((volatile T *)(X))

/**
 * Define an indirect read/write register at an aboslute address.
 * i.e. A register that is accessed through a pointer.
 *
 * @param X The address of the register.
 */
#define INDIRECT(X) INDIRECT_T(X, u8)

/**
 * Define a direct read/write variable at an aboslute address.
 *
 * @param X The address of the variable.
 * @param T The type of the variable.
 */
#define DIRECT_T(X, T) (*INDIRECT_T(X, T))

/**
 * Define a direct read/write register at an aboslute address.
 *
 * @param X The address of the register.
 */
#define DIRECT(X) DIRECT_T(X, u8)

/**
 * Take the low byte of a 16-bit value.
 *
 * @param X The 16-bit value.
 */
#define LO8(X) ((u8)X)

/**
 * Take the high byte of a 16-bit value.
 *
 * @param X The 16-bit value.
 */
#define HI8(X) LO8(X >> 8)

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
  ((T*) ((u8*) (1 ? (P) : &(((T*)0)->M)) - OFFSET_OF(M, T)))

#endif /* LZ_COMMON_H */

/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief stdio standard header file.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This is the standard libc stdio header file for the Lazuli system.
 */

#ifndef STDIO_H
#define STDIO_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Defines the End-Of-File constant.
 */
#define EOF ((int)-1)

/**
 * Produce unbuffered formatted output to the serial line.
 *
 * This function is fully reentrant.
 *
 * # Flag characters
 *
 * **0** The value should be zero-padded.
 *
 * # Field width
 *
 * A decimal digit string can be used to specify a minimum field width.
 *
 * # Conversion specifiers
 *
 * The following conversion specifiers are currently allowed:
 *
 * **d**, **i** The *int* argument is converted to decimal.
 *
 * **u**        The *unsigned int* argument is converted to decimal.
 *
 * **%**        The character '%' is output.
 *
 * @param format The format string.
 * @param ... The variadic parameters.
 *
 * @return The number of characters output to the serial line, or a negative
 *         value if an error occurred.
 *
 * @warning The stack usage of this function is important.
 *          No locking mechanism is provided.
 */
int
printf(const char * format, ...);

/**
 * Transmit a single character on the serial line.
 *
 * @param c The character to transmit, that will be cast to an unsigned char.
 *
 * @return The actual value of the transmitted character, resulting from the
 *         cast, as an int.
 *
 * @warning This operation is not thread safe.
 *          No locking mechanism is provided.
 */
int
putchar(int c);

/**
 * Transmit the NUL-terminated string @p s followed by a trailing newline on the
 * serial line, without any formatting.
 *
 * @param s The string to transmit.
 *
 * @return A non-negative number on success, otherwise EOF on error.
 *
 * @warning This operation is not thread safe.
 *          No locking mechanism is provided.
 */
int
puts(const char *s);

_EXTERN_C_DECL_END

#endif /* STDIO_H */

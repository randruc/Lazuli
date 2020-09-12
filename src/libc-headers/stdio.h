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
 * Produce formatted output to the serial line.
 *
 * This function is fully reentrant. No locking mechanism is provided.
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
 *         value if an error occured.
 *
 * @warning The stack usage of this function is important.
 */
int
printf(const char * format, ...);

_EXTERN_C_DECL_END

#endif /* STDIO_H */

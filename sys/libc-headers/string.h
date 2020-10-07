/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief libc string header.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file is the standard header <string.h> for the Lazuli RTOS.
 * It contains various declarations related to strings.
 */

#ifndef STRING_H
#define STRING_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Get the length of the string pointed by @p s.
 *
 * @param s A pointer to the string to get the length.
 *
 * @return The number of characters in the string @p s, excluding the final NUL
 *         character.
 *
 * @warning If @p s points to NULL, then the return value is 0.
 */
size_t
strlen(const char *s);

_EXTERN_C_DECL_END

#endif /* STRING_H */

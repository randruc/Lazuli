/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief libc string implementation.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file contains the implementation of various functions declared by the
 * standard header <string.h>.
 */

#include <string.h>

#include <Lazuli/common.h>

size_t
strlen(const char *s)
{
  const char * const original = s;

  if (NULL == original) {
    return 0;
  }

  while ('\0' != *(s++));

  --s; /* To exclude the count of the final '\0' */

  return s - original;
}

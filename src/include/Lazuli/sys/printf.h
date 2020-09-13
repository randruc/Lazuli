/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Internal functions needed for printf().
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file defines internal functions used by printf().
 */

#ifndef LAZULI_SYS_PRINTF_H
#define LAZULI_SYS_PRINTF_H

#include <stdint.h>

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Convert an unsigned 16-bit integer to its ASCII decimal representation.
 * @warning The conversion is put in the buffer @p buffer **in reverse order**
 * and **without a final NUL character**. The caller of this function then has
 * to use the return value in order to use the characters in the buffer in the
 * right order.
 *
 * This function will always succeed.
 *
 * @param i The 16-bit input value to convert.
 * @param buffer A valid pointer to an allocated string of minimum size 5 bytes.
 *
 * @return The number of characters actually written to the buffer.
 */
uint8_t
Printf_ConvertU16ToDecimal(uint16_t i, char buffer[]);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_PRINTF_H */

/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Basic type definitions and useful macros usable in ASM source files.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes basic type definitions and useful macros used in the
 * Lazuli project, and that can be used directly in ASM source files.
 */

#ifndef LAZULI_COMMON_ASM_H
#define LAZULI_COMMON_ASM_H

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

#endif /* LAZULI_COMMON_ASM_H */

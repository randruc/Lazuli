/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Kernel symbols definition usable in ASM source files.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file declares symbols related to the kernel itself, and that can be used
 * directly in ASM source files.
 */

#ifndef LAZULI_SYS_KERNEL_ASM_H
#define LAZULI_SYS_KERNEL_ASM_H

/**
 * The position of the flag constant for the status "in kernel".
 */
#define SYSTEM_STATUS_FLAG_IN_KERNEL_POSITION (0)

#endif /* LAZULI_SYS_KERNEL_ASM_H */

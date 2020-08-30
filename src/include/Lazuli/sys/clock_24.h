/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file src/include/Lazuli/sys/clock_24.h
 * @brief 24-Hour clock kernel interface.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the kernel interface of the 24-Hour clock.
 */

#ifndef LAZULI_SYS_CLOCK_24_H
#define LAZULI_SYS_CLOCK_24_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Increment the 24-Hour clock, taking into the account the resolution of the
 * system clock.
 */
void
Clock24_Increment(void);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_CLOCK_24_H */

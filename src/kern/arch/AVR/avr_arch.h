/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Header file specific to the AVR arch module.
 * @copyright 2018-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This header file declares all symbol neeed specifically in the AVR arch
 * module, and not declared in <Lazuli/sys/arch/arch.h>.
 */

#ifndef LAZULI_SRC_KERN_ARCH_AVR_ARCH_H
#define LAZULI_SRC_KERN_ARCH_AVR_ARCH_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Put the AVR CPU to sleep according to the sleep settings.
 */
extern void
cpu_sleep(void);

_EXTERN_C_DECL_END

#endif /* LAZULI_SRC_KERN_ARCH_AVR_ARCH_H */

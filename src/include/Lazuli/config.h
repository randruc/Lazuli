/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Include appropriate config file.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This header file is just here to include the appropriate config file if we
 * are performing static analysis.
 *
 * More info in Lazuli/config_static_analysis.h.
 */

#ifndef LAZULI_CONFIG_H
#define LAZULI_CONFIG_H

#ifdef LZ_STATIC_ANALYSIS
#include <Lazuli/config_static_analysis.h>
#else
#include <config.h>
#endif

#endif /* LAZULI_CONFIG_H */

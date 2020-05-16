/**
 * @file src/include/Lazuli/config.h
 * @brief Include appropriate config file.
 * @date May 2020
 * @author Remi Andruccioli
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

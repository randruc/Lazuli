/**
 * @file src/kern/modules/version_string/version_string.c
 * @brief Version string in final binary.
 * @date Mar 2020
 * @author Remi Andruccioli
 *
 * This module simply adds an ASCII string in the final binary containing the OS
 * name and version.
 */

#include <config.h>
#include <Lazuli/sys/compiler.h>

/**
 * Declare the version string to be included in the final binary.
 */
VERSION_STRING_SECTION char versionString[] = "{Lazuli-" LZ_VERSION "}";

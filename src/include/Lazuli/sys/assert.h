/**
 * @file src/include/Lazuli/sys/assert.h
 * @brief Perform assertions at runtime.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * Describes the macros needed to perform assertions at runtime.
 */

#ifndef LZ_SYS_ASSERT_H
#define LZ_SYS_ASSERT_H

#include <Lazuli/sys/config.h>
#include <Lazuli/sys/kernel.h>

/**
 * Common implementation to perform an assertion a runtime.
 * Should only be used within this file.
 *
 * If the assertion fails, invoke a kernel panic.
 *
 * @param assertion The assertion to check.
 * @param active A boolean to indicate if the assert is active.
 */
#define _BASE_ASSERT(assertion, active)         \
  if(!(active))                                 \
    {}                                          \
  else                                          \
    if(assertion)                               \
      {}                                        \
    else                                        \
      Panic()

/**
 * Perform an assertion a runtime, only in debug configuration.
 *
 * @param assertion The assertion to check.
 */
#define DEBUG_ASSERT(assertion) _BASE_ASSERT(assertion, LZ_DEBUG)

/**
 * Perform an assertion a runtime, wether the configuration is in debug or not.
 *
 * @param assertion The assertion to check.
 */
#define ALWAYS_ASSERT(assertion) _BASE_ASSERT(assertion, 1)

#endif /* LZ_SYS_ASSERT_H */

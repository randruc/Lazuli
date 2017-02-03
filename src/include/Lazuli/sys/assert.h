/**
 * @file assert.h
 *
 * Describes the macros needed to perform assertions at runtime.
 */

#ifndef LZ_ASSERT_H
#define LZ_ASSERT_H

#include <Lazuli/sys/config.h>
#include <Lazuli/sys/kernel.h>

/**
 * Common implementation to perform an assertion a runtime.
 * Should only be used within this file.
 *
 * If the assertion fails, invoke a kernel panic.
 *
 * @param C The assertion to check.
 */
#define _BASE_ASSERT(C)                         \
  if(C)                                         \
    {}                                          \
  else                                          \
    Panic()

#if DEBUG
/**
 * Perform an assertion a runtime, only in debug configuration.
 *
 * @param C The assertion to check.
 */
#define DEBUG_ASSERT(C) _BASE_ASSERT(C)
#else
#define DEBUG_ASSERT(C)
#endif /* DEBUG */

/**
 * Perform an assertion a runtime, wether the configuration is in debug or not.
 *
 * @param C The assertion to check.
 */
#define ALWAYS_ASSERT(C) _BASE_ASSERT(C)

#endif /* LZ_ASSERT_H */

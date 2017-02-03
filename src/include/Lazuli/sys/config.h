/**
 * @file config.h
 *
 * This file contains all the configuration of the kernel.
 * These are mainly constants.
 * Does not contain arch-specific configuration.
 */

#ifndef LZ_CONFIG_H
#define LZ_CONFIG_H

#include <Lazuli/common.h>

/**
 * Size in bytes of the security gap between the break and the stack pointer.
 */
#define BREAK_STACK_GAP ((u8)50)

/**
 * Clock frequency in Hertz.
 */
#define CLOCK_FREQUENCY (8000000)

/**
 * Run an infinite loop on kernel panic.
 */
#define ON_PANIC_INFINITE_LOOP (1)

/**
 * Perform a software reset on kernel panic.
 */
#define ON_PANIC_SOFTWARE_RESET (0)

#endif /* LZ_CONFIG_H */

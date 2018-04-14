/**
 * @file src/include/Lazuli/sys/config.h
 * @brief Kernel configuration.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file contains all the configuration for the kernel.
 * These are only macro constants.
 * Does not contain any arch-specific configuration.
 */

#ifndef LZ_SYS_CONFIG_H
#define LZ_SYS_CONFIG_H

/**
 * Size in bytes of the security gap between the break and the stack pointer.
 */
#define BREAK_STACK_GAP (50)

/**
 * Clock frequency in Hertz.
 */
#define CLOCK_FREQUENCY (1000000)

/**
 * When 1, run an infinite loop on kernel panic.
 *
 * When 0, doesn't apply.
 */
#define ON_PANIC_INFINITE_LOOP (1)

/**
 * When 1, perform a software reset on kernel panic.
 *
 * When 0, doesn't apply.
 */
#define ON_PANIC_SOFTWARE_RESET (0)

/**
 * Default stack size in bytes for a new task.
 */
#define DEFAULT_TASK_STACK_SIZE (20)

/**
 * When 1, always check for NULL function parameters in linked lists
 * implementation.
 *
 * When 0, never check for NULL parameters.
 * This is a way to obtain better performances, but it's also less safe.
 */
#define CHECK_NULL_PARAMETERS_IN_LISTS (1)

#endif /* LZ_SYS_CONFIG_H */

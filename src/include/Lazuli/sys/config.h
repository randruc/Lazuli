/**
 * @file config.h
 *
 * This file contains all configuration for the kernel.
 * These are only constants.
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
#define DEFAULT_TASK_STACK_SIZE (50)

/**
 * When 1, reset the kernel stack to its original position when running the
 * scheduler. This can be set to 1 only if calling the Run() function of the
 * scheduler is not expected to return.
 *
 * When 0, don't touch the kernel stack.
 */
#define RESET_KERNEL_STACK_ON_RUN (1)

#endif /* LZ_SYS_CONFIG_H */

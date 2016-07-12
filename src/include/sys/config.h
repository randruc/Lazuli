/**
 * @file config.h
 *
 * This file contains all the configuration of the kernel.
 * Does not contain arch-specific configuration.
 */

#ifndef CONFIG_H
#define CONFIG_H

/**
 * Size in bytes of the secutiry gap between the break and the stack pointer
 */
#define BREAK_STACK_GAP (50)

/**
 * Minimum size in bytes of a new alloc_block asked to sbrk
 */
#define MIN_ALLOC_BLOCK (100)

#endif /* CONFIG_H */

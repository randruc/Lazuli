/**
 * @file config.h
 *
 * This file contains all the configuration of the kernel.
 * Does not contain arch-specific configuration.
 */

#ifndef LZ_CONFIG_H
#define LZ_CONFIG_H

/**
 * Size in bytes of the secutiry gap between the break and the stack pointer.
 */
#define BREAK_STACK_GAP ((int)50)

#endif /* LZ_CONFIG_H */

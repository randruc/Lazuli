/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Kernel symbols definition.
 * @copyright 2017-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file declares symbols related to the kernel itself.
 */

#ifndef LAZULI_SYS_KERNEL_H
#define LAZULI_SYS_KERNEL_H

#include <stdint.h>

#include <Lazuli/common.h>

#include <Lazuli/sys/kernel_asm.h>
#include <Lazuli/sys/memory.h>

_EXTERN_C_DECL_BEGIN

/**
 * The type used for the system status "register".
 */
typedef uint8_t lz_system_status_t;

/**
 * System status "register" (bit field).
 */
extern volatile lz_system_status_t systemStatus;

/**
 * The flag constant for the status "in kernel".
 */
#define SYSTEM_STATUS_FLAG_IN_KERNEL \
  ((lz_system_status_t)POSITION(SYSTEM_STATUS_FLAG_IN_KERNEL_POSITION))

/**
 * Get a boolean value indicating if the system is curently in kernel mode.
 */
#define SYSTEM_STATUS_IS_IN_KERNEL (systemStatus & SYSTEM_STATUS_FLAG_IN_KERNEL)

/**
 * Allocation map for the kernel.
 */
extern AllocationMap kernelAllocationMap;

/**
 * Kernel panic.
 *
 * Take the appropriate decision according to the configuration: infinite loop,
 * reset the system.
 */
void
Kernel_Panic(void);

/**
 * Manage a failure that can happen in a function call.
 * Basically, this function does the following:
 * - If we are running in the context of a task, then abort the running task.
 * - If we are running in the kernel context, then kernel panic.
 *
 * This function can be used after testing parameters in functions that can be
 * called by both the kernel or user tasks (e.g. library functions).
 */
void
Kernel_ManageFailure(void);

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_KERNEL_H */

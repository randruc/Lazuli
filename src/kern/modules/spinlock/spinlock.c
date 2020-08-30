/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file src/kern/modules/spinlock/spinlock.c
 * @brief Spinlocks implementation
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the C part of the implementation of spinlocks.
 */

#include <Lazuli/common.h>
#include <Lazuli/config.h>
#include <Lazuli/spinlock.h>
#include <Lazuli/sys/task.h>

void
Lz_Spinlock_Unlock(Lz_Spinlock * const spinlock)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_SPINLOCKS) {
    if (NULL == spinlock) {
      Task_Abort();
    }
  }

  *spinlock = LZ_SPINLOCK_INIT;
}

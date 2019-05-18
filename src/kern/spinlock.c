/**
 * @file src/kern/spinlock.c
 * @brief Spinlocks implementation
 * @date Feb 2019
 * @author Remi Andruccioli
 *
 * This file describes the C part of the implementation of spinlocks.
 */

#include <Lazuli/common.h>
#include <Lazuli/spinlock.h>
#include <Lazuli/sys/config.h>
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

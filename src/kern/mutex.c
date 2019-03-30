/**
 * @file src/kern/mutex.c
 * @brief Mutex implementation.
 * @date Feb 2019
 * @author Remi Andruccioli
 *
 * This file describes the implementation of mutexes.
 * Mutexes in Lazuli are implemented as blocking.
 */

#include <Lazuli/mutex.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler_base.h>

void
Lz_Mutex_Init(Lz_Mutex * const mutex)
{
  const Lz_Mutex initValue = LZ_MUTEX_INIT;

  if (CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES) {
    if (NULL == mutex) {
      return;
    }
  }

  MemoryCopy(&initValue, mutex, sizeof(initValue));
}

void
Lz_Mutex_InitLocked(Lz_Mutex * const mutex)
{
  const Lz_Mutex initValue = LZ_MUTEX_INIT_LOCKED;

  if (CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES) {
    if (NULL == mutex) {
      return;
    }
  }

  MemoryCopy(&initValue, mutex, sizeof(initValue));
}

void
Lz_Mutex_Lock(Lz_Mutex * const mutex)
{
  if (CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES) {
    if (NULL == mutex) {
      Arch_InfiniteLoop();
    }
  }

  while (!Arch_TryAcquireLock(&(mutex->lock))) {
    Arch_WaitMutex(&(mutex->waitingTasks));
  }
}

void
Lz_Mutex_Unlock(Lz_Mutex * const mutex)
{
  if (CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES) {
    if (NULL == mutex) {
      Arch_InfiniteLoop();
    }
  }

  Arch_DisableInterrupts();
  mutex->lock = 0;
  BaseScheduler_WakeupTasksWaitingMutex(&(mutex->waitingTasks));
  Arch_EnableInterrupts();
}

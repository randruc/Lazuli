/**
 * @file src/kern/mutex.c
 * @brief Mutex implementation.
 * @date Feb 2019
 * @author Remi Andruccioli
 *
 * This file describes the implementation of mutexes.
 * Mutexes in Lazuli are implemented as blocking.
 */

#include <Lazuli/config.h>
#include <Lazuli/mutex.h>

#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler.h>
#include <Lazuli/sys/task.h>

void
Lz_Mutex_Init(Lz_Mutex * const mutex)
{
  const Lz_Mutex initValue = LZ_MUTEX_INIT;

  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES) {
    if (NULL == mutex) {
      Task_Abort();
    }
  }

  Memory_Copy(&initValue, mutex, sizeof(initValue));
}

void
Lz_Mutex_InitLocked(Lz_Mutex * const mutex)
{
  const Lz_Mutex initValue = LZ_MUTEX_INIT_LOCKED;

  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES) {
    if (NULL == mutex) {
      Task_Abort();
    }
  }

  Memory_Copy(&initValue, mutex, sizeof(initValue));
}

void
Lz_Mutex_Lock(Lz_Mutex * const mutex)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES) {
    if (NULL == mutex) {
      Task_Abort();
    }
  }

  while (!Arch_TryAcquireLock(&(mutex->lock))) {
    Arch_WaitMutex(&(mutex->waitingTasks));
  }
}

void
Lz_Mutex_Unlock(Lz_Mutex * const mutex)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES) {
    if (NULL == mutex) {
      Task_Abort();
    }
  }

  Arch_DisableInterrupts();
  mutex->lock = 0;
  Scheduler_WakeupTasksWaitingMutex(&(mutex->waitingTasks));
  Arch_EnableInterrupts();
}

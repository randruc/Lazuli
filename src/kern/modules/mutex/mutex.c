/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file src/kern/modules/mutex/mutex.c
 * @brief Mutex implementation.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
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

/**
 * Initialize a mutex in a given state.
 *
 * @param mutex A pointer to the mutex to initialize.
 * @param initValue A valid pointer to a mutex in the desired state.
 */
static void
InitMutex(Lz_Mutex * const mutex, const Lz_Mutex * const initValue)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES) {
    if (NULL == mutex) {
      Task_Abort();
    }
  }

  Memory_Copy(initValue, mutex, sizeof(Lz_Mutex));
}

/**
 * Set the task to wait for a given mutex to be unlocked.
 *
 * @param mutex a valid pointer to the mutex to wait.
 */
static void
WaitMutex(Lz_Mutex * const mutex)
{
  Task * const currentTask = Scheduler_GetCurrentTask();

  currentTask->taskToSchedulerMessage = WAIT_MUTEX;
  currentTask->taskToSchedulerMessageParameter = mutex;

  Scheduler_SleepUntilEndOfTimeSlice();
}

/** @name User API */
/** @{             */

void
Lz_Mutex_Init(Lz_Mutex * const mutex)
{
  const Lz_Mutex initValueUnlocked = LZ_MUTEX_INIT;

  InitMutex(mutex, &initValueUnlocked);
}

void
Lz_Mutex_InitLocked(Lz_Mutex * const mutex)
{
  const Lz_Mutex initValueLocked = LZ_MUTEX_INIT_LOCKED;

  InitMutex(mutex, &initValueLocked);
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
    WaitMutex(mutex);
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
  Scheduler_WakeupTasksWaitingMutex(mutex);
  Arch_EnableInterrupts();
}

/** @} */

/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file src/include/Lazuli/mutex.h
 * @brief Mutexes interface.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * Describes the interface for mutexes.
 * Mutexes in Lazuli are implemented as blocking.
 */

#ifndef LAZULI_MUTEX_H
#define LAZULI_MUTEX_H

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/list.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents a mutex.
 */
typedef struct {
  volatile uint8_t lock;      /**< The mutex lock                           */
  Lz_LinkedList waitingTasks; /**< The list of tasks waiting for that mutex */
}Lz_Mutex;

/**
 * Define the value to initialize a Lz_Mutex in the unlocked state.
 *
 * This macro constant must be used to statically initialize a declared
 * mutex.
 */
#define LZ_MUTEX_INIT { 0, LINKED_LIST_INIT }

/**
 * Define the value to initialize a Lz_Mutex in the locked state.
 *
 * This macro constant must be used to statically initialize a declared
 * mutex.
 */
#define LZ_MUTEX_INIT_LOCKED { 1, LINKED_LIST_INIT }

/**
 * Initialize an already allocated Lz_Mutex.
 * The mutex will be initialized in an unlocked state.
 *
 * @param mutex A pointer to the Lz_Mutex to initialize.
 *
 * @note The calling task will abort if configuration macro
 *       LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES is set to 1 and the
 *       parameter @p mutex is _NULL_.
 */
void
Lz_Mutex_Init(Lz_Mutex * const mutex);

/**
 * Initialize an already allocated Lz_Mutex.
 * The mutex will be initialized in a locked state.
 *
 * @param mutex A pointer to the Lz_Mutex to initialize.
 *
 * @note The calling task will abort if configuration macro
 *       LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES is set to 1 and the
 *       parameter @p mutex is _NULL_.
 */
void
Lz_Mutex_InitLocked(Lz_Mutex * const mutex);

/**
 * Lock the mutex and enter critical section.
 *
 * @param mutex A pointer to the Lz_Mutex to lock.
 *
 * @note The calling task will abort if configuration macro
 *       LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES is set to 1 and the
 *       parameter @p mutex is _NULL_.
 */
void
Lz_Mutex_Lock(Lz_Mutex * const mutex);

/**
 * Unlock the mutex and leave critical section.
 *
 * @param mutex A pointer to the Lz_Mutex to unlock.
 *
 * @note The calling task will abort if configuration macro
 *       LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES is set to 1 and the
 *       parameter @p mutex is _NULL_.
 */
void
Lz_Mutex_Unlock(Lz_Mutex * const mutex);

_EXTERN_C_DECL_END

#endif /* LAZULI_MUTEX_H */

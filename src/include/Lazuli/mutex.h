/**
 * @file src/include/Lazuli/mutex.h
 * @brief Mutexes interface.
 * @date Feb 2019
 * @author Remi Andruccioli
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
  volatile uint8_t lock;   /**< The mutex lock                           */
  LinkedList waitingTasks; /**< The list of tasks waiting for that mutex */
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
 */
void
Lz_Mutex_Init(Lz_Mutex * const mutex);

/**
 * Initialize an already allocated Lz_Mutex.
 * The mutex will be initialized in a locked state.
 *
 * @param mutex A pointer to the Lz_Mutex to initialize.
 */
void
Lz_Mutex_InitLocked(Lz_Mutex * const mutex);

/**
 * Lock the mutex and enter critical section.
 *
 * @param mutex A pointer to the Lz_Mutex to lock.
 *              If the pointer mutex is NULL, then the call to this function
 *              will spin forever.
 */
void
Lz_Mutex_Lock(Lz_Mutex * const mutex);

/**
 * Unlock the mutex and leave critical section.
 *
 * @param mutex A pointer to the Lz_Mutex to unlock.
 *              If the pointer mutex is NULL, then the call to this function
 *              will spin forever.
 */
void
Lz_Mutex_Unlock(Lz_Mutex * const mutex);

_EXTERN_C_DECL_END

#endif /* LAZULI_MUTEX_H */

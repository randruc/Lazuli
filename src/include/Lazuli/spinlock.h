/**
 * @file src/include/Lazuli/spinlock.h
 * @brief Spinlocks interface.
 * @date Feb 2019
 * @author Remi Andruccioli
 *
 * This file describes the interface for spinlocks.
 */

#ifndef LAZULI_SPINLOCK_H
#define LAZULI_SPINLOCK_H

#include <stdint.h>

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Type used to represent a spinlock.
 */
typedef volatile uint8_t Lz_Spinlock;

/**
 * Define the initialization value for the type Lz_Spinlock.
 *
 * This macro constant must be used to statically initialize a declared
 * spinlock.
 */
#define LZ_SPINLOCK_INIT ((uint8_t)0)

/**
 * Set the lock and enter region, or active wait if the lock is already set.
 *
 * @param spinlock A pointer to a Lz_Spinlock used to hold the lock.
 *                 If the pointer lock is NULL, then the call to this function
 *                 will spin forever.
 */
void
Lz_Spinlock_Lock(Lz_Spinlock * const spinlock);

/**
 * Unset the lock and leave region.
 *
 * @param spinlock A pointer to a Lz_Spinlock used to hold the lock.
 *                 If the pointer lock is NULL, then the call to this function
 *                 will spin forever.
 */
void
Lz_Spinlock_Unlock(Lz_Spinlock * const spinlock);

_EXTERN_C_DECL_END

#endif /* LAZULI_SPINLOCK_H */

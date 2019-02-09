/**
 * @file src/include/Lazuli/spinlock.h
 * @brief Spinlocks interface.
 * @date Feb 2019
 * @author Remi Andruccioli
 *
 * Describes the interface for spinlocks.
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
 * Defines the initialization value of a spinlock.
 * The macro constant must be used to statically initialize a declared spinlock.
 */
#define LZ_SPINLOCK_INIT ((uint8_t)0)

/**
 * Set the lock and enter region.
 *
 * @param lock A pointer to a Lz_Spinlock used to hold the lock.
 *             If the pointer lock is NULL, then the call to this function will
 *             spin forever.
 */
void
Lz_Spinlock_Lock(Lz_Spinlock * const lock);

/**
 * Unseet the lock and leave region.
 *
 * @param lock A pointer to a Lz_Spinlock used to hold the lock.
 *             If the pointer lock is NULL, then the call to this function will
 *             spin forever.
 */
void
Lz_Spinlock_Unlock(Lz_Spinlock * const lock);

_EXTERN_C_DECL_END

#endif /* LAZULI_SPINLOCK_H */

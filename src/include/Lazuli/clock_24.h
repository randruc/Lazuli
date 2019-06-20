/**
 * @file src/include/Lazuli/clock_24.h
 * @brief 24-Hour clock user interface.
 * @date Jun 2019
 * @author Remi Andruccioli
 *
 * This file describes the user interface of the 24-Hour clock.
 */

#ifndef LAZULI_CLOCK_24_H
#define LAZULI_CLOCK_24_H

#include <stdint.h>

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents the type used to hold the time.
 */
typedef struct {
  uint8_t hours;   /**< Hours   */
  uint8_t minutes; /**< Minutes */
  uint8_t seconds; /**< Seconds */
}Clock24;

/**
 * Get the time on a 24-Hour clock.
 *
 * @param userClock24 A pointer to an allocated Clock24.
 *
 * @warning This function may spin if the kernel updates the time while this
 * functions reads it.
 */
void
Lz_Clock24_Get(Clock24 * const userClock24);

_EXTERN_C_DECL_END

#endif /* LAZULI_CLOCK_24_H */

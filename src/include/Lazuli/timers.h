/**
 * @file src/include/Lazuli/timers.h
 * @brief The public API of timers.
 * @date Jul 2018
 * @author Remi Andruccioli
 *
 * This file describes the public API of virtual timers.
 */

#ifndef LAZULI_TIMERS_H
#define LAZULI_TIMERS_H

#include <stdint.h>

#include <Lazuli/list.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents the context of a virtual timer.
 *
 * This object is used to hold and manage the memory needed for the declaration
 * of a virtual timer.
 */
typedef struct {
  /**
   * The linked list used to store the timer.
   */
  LinkedListElement linkedList;

  /**
   * The duration, relative to the previous timer in the list.
   */
  uint32_t duration;
}Lz_TimerContext;

_EXTERN_C_DECL_END

#endif /* LAZULI_TIMERS_H */

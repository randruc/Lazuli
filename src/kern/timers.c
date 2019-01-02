/**
 * @file src/kern/timers.c
 * @brief Virtual timers implementation.
 * @date Jul 2018
 * @author Remi Andruccioli
 *
 * This file describes the implementation of virtual timers.
 */

#include <stdint.h>

#include <Lazuli/list.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/timers.h>

/**
 * The linked list of current virtual timers.
 */
static LinkedList timers = LINKED_LIST_INIT;

/* TODO: Add locks if necessary */
void
DeclareVirtualTimer(Lz_TimerContext * const timerContext,
                    uint32_t desiredDuration)
{
  Lz_TimerContext *loopTimer;
  LinkedListElement *timerContextLinkedListElement;

  if (NULL == timerContext) {
    return;
  }

  timerContextLinkedListElement = &(timerContext->linkedList);

  List_ForEach(&timers, Lz_TimerContext, loopTimer, linkedList) {
    if (desiredDuration < loopTimer->duration) {
      loopTimer->duration -= desiredDuration;
      List_InsertBefore(&timers,
                        &(loopTimer->linkedList),
                        timerContextLinkedListElement);

      if (List_IsFirstElement(&timers, timerContextLinkedListElement)) {
        Arch_SetHardwareTimer(desiredDuration);
      }

      return;
    }

    desiredDuration -= loopTimer->duration;
  }

  List_Append(&timers, timerContextLinkedListElement);

  if (List_IsFirstElement(&timers, timerContextLinkedListElement)) {
    Arch_SetHardwareTimer(desiredDuration);
  }
}

/* TODO: Add locks if necessary */
void
SetNextVirtualTimer()
{
  LinkedListElement *nextTimer;
  Lz_TimerContext *timerContext;

  List_PickFirst(&timers);

  nextTimer = List_PointFirst(&timers);
  if (NULL == nextTimer) {
    return;
  }

  timerContext = CONTAINER_OF(nextTimer, linkedList, Lz_TimerContext);

  Arch_SetHardwareTimer(timerContext->duration);
}

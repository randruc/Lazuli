/**
 * @file src/kern/arch/AVR/timer_counter_0.c
 *
 * Describes the driver implementation for the "8-bit timer/counter 0" of the
 * ATMega328p.
 */

#include <Lazuli/common.h>
#include <Lazuli/sys/arch/AVR/timer_counter_0.h>

/* TODO: Check if we can return a const object. */
TimerCounter0 *
GetTimerCounter0()
{
  return (TimerCounter0 *)&TCCR0A;
}

void
TimerCounter0InterruptsEnable(const u8 interruptsMask)
{
  SET_BITS(TIMSK0, u8, interruptsMask);
}

void
TimerCounter0InterruptsDisable(const u8 interruptsMask)
{
  CLEAR_BITS(TIMSK0, u8, interruptsMask);
}

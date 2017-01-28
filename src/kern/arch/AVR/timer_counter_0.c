/**
 * @file timer_counter_0.c
 *
 * Describes the driver implementation for the "8-bit timer/counter 0" of the 
 * atmega328p.
 */

#include <sys/arch/AVR/timer_counter_0.h>

TimerCounter0 *
GetTimerCounter0()
{
  return (TimerCounter0 *)&TCCR0A;
}

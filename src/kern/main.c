/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <Lazuli/common.h>
#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/arch/AVR/arch.h>
#include <Lazuli/sys/arch/AVR/timer_counter_0.h>

void
Int0Handler()
{
}

void
Int1Handler()
{
}

void
Timer0CompareMatchAHandler()
{
  PORTB = ~PORTB;
}

void
TimerCounter0OverflowHandler()
{
}

/**
 * Main entry point for user tasks.
 */
void
Main()
{
  TimerCounter0 * const timer0 = GetTimerCounter0();

  DDRB = (u8)0xff;
  EICRA = (u8)0x03;
  EIMSK = (u8)0x01;

  PORTB = u8_MIN;

  timer0->tccr0a = TCCR0A_WGM01;
  timer0->tccr0b = TCCR0B_CS02 | TCCR0B_CS00;
  timer0->tcnt0 = (u8)0;
  timer0->ocr0a = (u8)20;

  TimerCounter0InterruptsEnable(TIMSK0_OCIE0A);
  GlobalInterruptsEnable();

  while(true);
}

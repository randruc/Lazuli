/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <sys/arch/AVR/registers.h>
#include <sys/arch/AVR/arch.h>
#include <sys/types.h>
#include <sys/arch/AVR/timer_counter_0.h>

void
Int0Handler()
{
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
  timer0->ocr0a = (u8)80;
  timer0->tcnt0 = (u8)0;

  while (true) {
    if (TIFR0 & (u8)TIFR0_OCF0A) {
      PORTB = ~PORTB;
      SET_BITS(TIFR0, u8, TIFR0_OCF0A);
    }
  }

  /*
   * TimerCounter0InterruptsEnable(TIMSK0_TOEI0);
   * GlobalInterruptsEnable();
   */

}

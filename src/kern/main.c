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

/* 
 * void
 * Timer0CompareMacthAHandler()
 * {
 *   mode = !mode;
 *   
 *   if (mode) {
 * 	PORTB = u8_MAX;
 *   } else {
 * 	PORTB = u8_MIN;
 *   }
 * }
 */

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

  timer0->tccr0b = 0;
  timer0->tccr0b = TCCR0B_CS02 | TCCR0B_CS00;
  timer0->tcnt0 = 0;

  PORTB = u8_MIN;
  
  while (true) {
	if (TCNT0 >= 120) {
	  PORTB = u8_MAX;
	} else {
	  PORTB = u8_MIN;
	}
  }
}

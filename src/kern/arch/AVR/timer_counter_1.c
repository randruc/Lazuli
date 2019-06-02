/**
 * @file src/kern/arch/AVR/timer_counter_1.c
 * @brief ATMega328p Timer/Counter 1 driver.
 * @date May 2019
 * @author Remi Andruccioli
 *
 * This file describes the driver implementation for the
 * "16-bit timer counter 1" of the ATMega328p.
 */

#include <stdint.h>

#include <Lazuli/sys/arch/AVR/timer_counter_1.h>

void
Arch_InitSystemTimer(void)
{
  /*
   * Setting for a timer resolution of 10ms, for a system clock of 1 MHz.
   */
  const uint16_t compareMatchRegisterValue = 4999;

  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= TCCR1B_WGM12; /* CTC mode, TOP is OCR1A */
  TIMSK1 = 0;
  TIFR1 = 0;
  TCNT1H = 0;
  TCNT1L = 0;
  OCR1AH = HI8(compareMatchRegisterValue);
  OCR1AL = LO8(compareMatchRegisterValue);
}

void
Arch_StartSystemTimer(void)
{
  TIMSK1 |= TIMSK1_OCIE1A; /* Enable output compare A match interrupt */
  TCCR1B |= TCCR1B_CS10;   /* Clock select : system clock, prescale by 1 */
}

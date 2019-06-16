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
#include <Lazuli/sys/config.h>

/** The Timer/Counter 1 clock prescaler */
#define TIMER_COUNTER_1_PRESCALER (8)

/**
 * Value of the compare match register whith the desired clock settings.
 */
#define COMPARE_MATCH_REGISTER_VALUE                                    \
  ((uint16_t)((LZ_CONFIG_MACHINE_CLOCK_FREQUENCY /                      \
               (TIMER_COUNTER_1_PRESCALER *                             \
                LZ_CONFIG_SYSTEM_CLOCK_RESOLUTION_FREQUENCY)) - 1))     \

void
Arch_InitSystemTimer(void)
{
  const uint16_t compareMatchRegisterValue = COMPARE_MATCH_REGISTER_VALUE;

  TCCR1A = 0;
  TIMSK1 = 0;
  TIFR1 = 0;
  TCNT1H = 0;
  TCNT1L = 0;
  TCCR1B = TCCR1B_WGM12; /* CTC mode, TOP is OCR1A */
  OCR1AH = HI8(compareMatchRegisterValue);
  OCR1AL = LO8(compareMatchRegisterValue);
}

void
Arch_StartSystemTimer(void)
{
  /* Enable output compare A match interrupt */
  TIMSK1 |= TIMSK1_OCIE1A;

  /* Clock select : system clock, prescale by 8 */
  TCCR1B |= TCCR1B_CS11;
}

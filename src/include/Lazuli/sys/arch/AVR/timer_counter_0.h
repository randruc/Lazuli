/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file src/include/Lazuli/sys/arch/AVR/timer_counter_0.h
 * @brief Interface for Timer/Counter 0 of the ATMega328p.
 * @copyright 2017-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * Describes the interface of the "8-bit timer/counter 0" driver for the
 * ATMega328p.
 */

#ifndef LAZULI_SYS_ARCH_AVR_TIMER_COUNTER_0_H
#define LAZULI_SYS_ARCH_AVR_TIMER_COUNTER_0_H

#include <stdint.h>

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/*
 * Timer 0 registers definitions.
 * Ordered by address.
 */

/** Timer/Counter 0 Interrupt Flag Register */
#define TIFR0  DIRECT(0x35)

/** Timer/Counter Control Register A        */
#define TCCR0A DIRECT(0x44)

/** Timer/Counter Control Register B        */
#define TCCR0B DIRECT(0x45)

/** Timer/Counter Register                  */
#define TCNT0  DIRECT(0x46)

/** Output Compare register A               */
#define OCR0A  DIRECT(0x47)

/** Output Compare register B               */
#define OCR0B  DIRECT(0x48)

/** Timer/Counter Interrupt Mask Register   */
#define TIMSK0 DIRECT(0x6e)

/**
 * Describes the memory mapping for timer/counter 0.
 */
typedef struct {
  volatile uint8_t tccr0a; /**< Timer/Counter Control Register A        */
  volatile uint8_t tccr0b; /**< Timer/Counter Control Register B        */
  volatile uint8_t tcnt0;  /**< Timer/Counter Register                  */
  volatile uint8_t ocr0a;  /**< Output Compare register A               */
  volatile uint8_t ocr0b;  /**< Output Compare register B               */
}TimerCounter0;

/**
 * Content of the TCCR0A register.
 */
enum {
  TCCR0A_WGM00  = POSITION(0U), /**< Waveform Generation Mode    */
  TCCR0A_WGM01  = POSITION(1U), /**< Waveform Generation Mode    */
  TCCR0A_COM0B0 = POSITION(4U), /**< Compare Match Output B Mode */
  TCCR0A_COM0B1 = POSITION(5U), /**< Compare Match Output B Mode */
  TCCR0A_COM0A0 = POSITION(6U), /**< Compare Match Output A Mode */
  TCCR0A_COM0A1 = POSITION(7U)  /**< Compare Match Output A Mode */
};

/**
 * Content of the TCCR0B register
 */
enum {
  TCCR0B_CS00  = POSITION(0U), /**< Clock Select             */
  TCCR0B_CS01  = POSITION(1U), /**< Clock Select             */
  TCCR0B_CS02  = POSITION(2U), /**< Clock Select             */
  TCCR0B_WGM02 = POSITION(3U), /**< Waveform Generation Mode */
  TCCR0B_FOC0B = POSITION(6U), /**< Force Output Compare B   */
  TCCR0B_FOC0A = POSITION(7U)  /**< Force Output Compare A   */
};

/**
 * Content of the TIMSK0 register.
 */
enum {
  /** Timer/Counter 0 Overflow Interrupt enable */
  TIMSK0_TOIE0 = POSITION(0U),

  /** Timer/Counter 0 Output Compare Match A Interrupt Enable */
  TIMSK0_OCIE0A = POSITION(1U),

  /** Timer/Counter 0 Output Compare Match B Interrupt Enable */
  TIMSK0_OCIE0B = POSITION(2U)
};

/**
 * Content of the TIFR0 Register.
 */
enum {
  TIFR0_TOV0  = POSITION(0U), /**< Timer/Counter 0 Overflow Flag             */
  TIFR0_OCF0A = POSITION(1U), /**< Timer/Counter 0 Output Compare A Match
                                  Flag                                      */
  TIFR0_OCF0B = POSITION(2U)  /**< Timer/Counter 0 Output Compare B Match
                                  Flag                                      */
};

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_ARCH_AVR_TIMER_COUNTER_0_H */

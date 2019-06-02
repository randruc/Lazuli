/**
 * @file src/include/Lazuli/sys/arch/AVR/timer_counter_1.h
 * @brief Interface for Timer/Counter 1 of the ATMega328p.
 * @date May 2019
 * @author Remi Andruccioli
 *
 * This file describes the interface of the driver for the
 * "16-bit timer counter 1" of the ATMega328p.
 */

#ifndef LAZULI_SYS_ARCH_AVR_TIMER_COUNTER_1_H
#define LAZULI_SYS_ARCH_AVR_TIMER_COUNTER_1_H

#include <stdint.h>

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * @name Timer/Counter 1 registers definitions.
 * Ordered by address.
 *
 * @{
 */

/** Timer/Counter1 Control Register A */
#define TCCR1A DIRECT(0x80)

/** Timer/Counter1 Control Register B */
#define TCCR1B DIRECT(0x81)

/** Timer/Counter1 Control Register C */
#define TCCR1C DIRECT(0x82)

/** Timer/Counter1 (Low register) */
#define TCNT1L DIRECT(0x84)

/** Timer/Counter1 (High register) */
#define TCNT1H DIRECT(0x85)

/** Timer/Counter1 (16-bit register) */
#define TCNT1 DIRECT_T(0x84, uint16_t)

/** Input Capture Register 1 (Low register) */
#define ICR1L DIRECT(0x86)

/** Input Capture Register 1 (High register)*/
#define ICR1H DIRECT(0x87)

/** Output Compare Register 1 A (Low register) */
#define OCR1AL DIRECT(0x88)

/** Output Compare Register 1 A (High register) */
#define OCR1AH DIRECT(0x89)

/** Output Compare Register 1 B (Low register) */
#define OCR1BL DIRECT(0x8a)

/** Output Compare Register 1 B (High register) */
#define OCR1BH DIRECT(0x8b)

/** Timer/Counter 1 Interrupt Mask Register */
#define TIMSK1 DIRECT(0x6f)

/** Timer/Counter 1 Interrupt Flag Register */
#define TIFR1 DIRECT(0x36)

/** @} */

/**
 * @name Timer/Counter 1 registers content.
 *
 * @{
 */

/**
 * Content of the TCCR1A register.
 */
enum {
  TCCR1A_WGM10  = POSITION(0), /**< Waveform generation mode          */
  TCCR1A_WGM11  = POSITION(1), /**< Waveform generation mode          */
  TCCR1A_COM1B0 = POSITION(4), /**< Compare output mode for Channel B */
  TCCR1A_COM1B1 = POSITION(5), /**< Compare output mode for Channel B */
  TCCR1A_COM1A0 = POSITION(6), /**< Compare output mode for Channel A */
  TCCR1A_COM1A1 = POSITION(7)  /**< Compare output mode for Channel A */
};

/**
 * Content of the TCCR1B register.
 */
enum {
  TCCR1B_CS10  = POSITION(0), /**< Clock select                  */
  TCCR1B_CS11  = POSITION(1), /**< Clock select                  */
  TCCR1B_CS12  = POSITION(2), /**< Clock select                  */
  TCCR1B_WGM12 = POSITION(3), /**< Waveform generation mode      */
  TCCR1B_WGM13 = POSITION(4), /**< Waveform generation mode      */
  TCCR1B_ICES1 = POSITION(6), /**< Input capture edge select     */
  TCCR1B_ICNC1 = POSITION(7)  /**< Input capture noise canceller */
};

/**
 * Content of the TCCR1C register.
 */
enum {
  TCCR1C_FOC1B = POSITION(6), /**< Force output compare for channel A */
  TCCR1C_FOC1A = POSITION(7)  /**< Force output compare for channel B */
};

/**
 * Content of the TIMSK1 register.
 */
enum {
  TIMSK1_TOIE1  = POSITION(0),  /**< Overflow interrupt enable               */
  TIMSK1_OCIE1A = POSITION(1),  /**< Output compare A match interrupt enable */
  TIMSK1_OCIE1B = POSITION(2),  /**< Output compare B match interrupt enable */
  TIMSK1_ICIE1  = POSITION(5)   /**< Input capture enable                    */
};

/**
 * Content of the TIFR1 register.
 */
enum {
  TIFR1_TOV1  = POSITION(0), /**< Overflow flag               */
  TIFR1_OCF1A = POSITION(1), /**< Output compare A match flag */
  TIFR1_OCF1B = POSITION(2), /**< Output compare B match flag */
  TIFR1_ICF1  = POSITION(5)  /**< Input capture flag          */
};

/** @} */

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_ARCH_AVR_TIMER_COUNTER_1_H */

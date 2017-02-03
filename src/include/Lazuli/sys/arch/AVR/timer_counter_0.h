/**
 * @file timer_counter_0.h
 *
 * Describes the interface of the "8-bit timer/counter 0" driver for the
 * atmega328p.
 */

#ifndef LZ_AVR_TIMER_COUNTER_0_H
#define LZ_AVR_TIMER_COUNTER_0_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/*
 * Timer 0 registers definitions
 * Ordered by address.
 */
#define TIFR0  DIRECT(0x35)   /** < Timer/Counter 0 Interrupt Flag Register */
#define TCCR0A DIRECT(0x44)   /** < Timer/Counter Control Register A        */
#define TCCR0B DIRECT(0x45)   /** < Timer/Counter Control Register B        */
#define TCNT0  DIRECT(0x46)   /** < Timer/Counter Register                  */
#define OCR0A  DIRECT(0x47)   /** < Output Compare register A               */
#define OCR0B  DIRECT(0x48)   /** < Output Compare register B               */
#define TIMSK0 DIRECT(0x6e)   /** < Timer/Counter Interrupt Mask Register   */

/**
 * Describes the memory mapping for timer/counter 0.
 */
typedef struct {
  volatile u8 tccr0a; /** < Timer/Counter Control Register A        */
  volatile u8 tccr0b; /** < Timer/Counter Control Register B        */
  volatile u8 tcnt0;  /** < Timer/Counter Register                  */
  volatile u8 ocr0a;  /** < Output Compare register A               */
  volatile u8 ocr0b;  /** < Output Compare register B               */
}TimerCounter0;

/**
 * Content of the TCCR0A register.
 */
enum {
  TCCR0A_WGM00  = POSITION(0), /** < Waveform Generation Mode    */
  TCCR0A_WGM01  = POSITION(1), /** < Waveform Generation Mode    */
  TCCR0A_COM0B0 = POSITION(4), /** < Compare Match Output B Mode */
  TCCR0A_COM0B1 = POSITION(5), /** < Compare Match Output B Mode */
  TCCR0A_COM0A0 = POSITION(6), /** < Compare Match Output A Mode */
  TCCR0A_COM0A1 = POSITION(7)  /** < Compare Match Output A Mode */
};

/**
 * Content of the TCCR0B register
 */
enum {
  TCCR0B_CS00  = POSITION(0), /** < Clock Select             */
  TCCR0B_CS01  = POSITION(1), /** < Clock Select             */
  TCCR0B_CS02  = POSITION(2), /** < Clock Select             */
  TCCR0B_WGM02 = POSITION(3), /** < Waveform Generation Mode */
  TCCR0B_FOC0B = POSITION(6), /** < Force Output Compare B   */
  TCCR0B_FOC0A = POSITION(7)  /** < Force Output Compare A   */
};

/**
 * Content of the TIMSK0 register.
 */
enum {
  /** Timer/Counter 0 Overflow Interrupt enable */
  TIMSK0_TOEI0 = POSITION(0),

  /** Timer/Counter 0 Output Compare Match A Interrupt Enable */
  TIMSK0_OCIE0A = POSITION(1),

  /** Timer/Counter 0 Output Compare Match B Interrupt Enable */
  TIMSK0_OCIE0B = POSITION(2)
};

/**
 * Content of the TIFR0 Register.
 */
enum {
  TIFR0_TOV0  = POSITION(0), /** < Timer/Counter 0 Overflow Flag             */
  TIFR0_OCF0A = POSITION(1), /** < Timer/Counter 0 Output Compare A Match
                                   Flag                                      */
  TIFR0_OCF0B = POSITION(2)  /** < Timer/Counter 0 Output Compare B Match
                                   Flag                                      */
};

/**
 * Obtain the memory mapped structure of the "8-bit timer/counter 0" driver.
 *
 * @return A pointer to the memory mapped structure.
 */
TimerCounter0 *
GetTimerCounter0();

/**
 * Enable interrupts fortimer/counter 0.
 *
 * @param interruptsMask The mask of interrupts to enable, according to the
 *                       possible values of TIMSK0 register.
 */
void
TimerCounter0InterruptsEnable(const u8 interruptsMask);

/**
 * Disable interrupts for timer/counter 0.
 *
 * @param interruptsMask The mask of interrupts to disable, according to the
 *                       possible values of TIMSK0 register.
 */
void
TimerCounter0InterruptsDisable(const u8 interruptsMask);

_EXTERN_C_DECL_END

#endif /* LZ_AVR_TIMER_COUNTER_0_H */

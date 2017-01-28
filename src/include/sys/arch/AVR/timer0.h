/**
 * @file timer0.h
 *
 * Describes the interface of the "8-bit timer/counter 0" driver for the
 * atmega328p.
 */

#ifndef LZ_ATMEGA328P_TIMER_0_H
#define LZ_ATMEGA328P_TIMER_0_H

#include <sys/types.h>

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
  TIMSK0_TOEIE0 = POSITION(0), /** < Timer/Counter 0 Overflow Interrupt
								     enable                                  */
  TIMSK0_OCIE0A = POSITION(1), /** < Timer/Counter 0 Output Compare Match A
								     Interrupt Enable                        */
  TIMSK0_OCIE0B = POSITION(2)  /** < Timer/Counter 0 Output Compare Match B
								     Interrupt Enable                        */
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
 * Enumerates the modes of operation for timer 0.
 */
typedef enum {
  Normal              = 0, /** < Normal                          */
  PwmPhaseCorrect0xFF = 1, /** < PWM, phase correct, top on 0xFF */
  CTC                 = 2, /** < Clear timer on compare match    */
  FastPwm0xFF         = 3, /** < Fast PWM, top on 0xFF           */
  PWMPhaseCorrect     = 5, /** < PWM, phase correct, top on OCRA */
  FastPwm             = 7  /** < Fast PWM, top on OCRA           */
}Timer0Mode;

/**
 * Enumerates the clock sources for timer 0.
 */
typedef enum {
  Stop = 0,          /** < No clock source (timer stopped)    */
  SystemClock,       /** < System clock without prescaling    */
  SystemClock_8,     /** < System clock prescaled by 8        */
  SystemClock_64,    /** < System clock prescaled by 64       */
  SystemClock_256,   /** < System clock prescaled by 256      */
  SystemClock_1024,  /** < System clock prescaled by 1024     */
  ExternalT0Falling, /** < External clock on T0, falling edge */
  ExternalT0Rising   /** < External clock on T0, rising edge  */
}Timer0ClockSelect;

/**
 * Select clock source of timer 0.
 *
 * @param clockSelect The desired clock source.
 */
void
Timer0SelectClock(const Timer0ClockSelect clockSelect);

/**
 * Stop timer 0.
 */
void
Timer0Stop();

/**
 * Set the mode of operation for timer 0.
 *
 * @param mode The desired mode of operation.
 */
void
Timer0SetMode(const Timer0Mode mode);

/* TODO: Complete comment */
/**
 * Set the value of the compare register A of timer 0.
 */
void
Timer0SetCompareRegisterA(const u8 value);

/**
 * Enable timer 0 module (needed for power reduction).
 */
void
Timer0Enable();

/**
 * Disable timer 0 module (needed for power reduction).
 */
void
Timer0Disable();

/**
 * Enable interrupts when a compare A match occurs on timer 0.
 */
void
Timer0OutputCompareMatchAInterruptEnable();

/**
 * Disable interrupts when a compare A match occurs on timer 0.
 */
void
Timer0OutputCompareMatchAInterruptDisable();

_EXTERN_C_DECL_END

#endif /* LZ_ATMEGA328P_TIMER_0_H */

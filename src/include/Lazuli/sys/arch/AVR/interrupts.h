/**
 * @file src/include/Lazuli/sys/arch/AVR/interrupts.h
 * @brief Interrupt codes.
 * @date Mar 2018
 * @author Remi Andruccioli
 * @warning Used in both C and ASM code.
 *
 * This file describes the codes associated to each interrupt of the AVR
 * platform.
 * They are mainly used to perform a jump in a function pointer table in the
 * scheduler.
 * Hence, the first entry MUST be 0, and the following incremented by 1.
 * We can't use a C enum here because these interrupt codes need to be
 * manipulated in ASM code.
 */

#ifndef LZ_SYS_ARCH_AVR_INTERRUPTS_H
#define LZ_SYS_ARCH_AVR_INTERRUPTS_H

/**
 * Code for interrupt: Reset, Entry point at boot.
 */
#define INT_RESET (0)

/**
 * Code for interrupt: External interrupt request 0.
 */
#define INT_INT0 (1)

/**
 * Code for interrupt: External interrupt request 1.
 */
#define INT_INT1 (2)

/**
 * Code for interrupt: Pin change interrupt request 0.
 */
#define INT_PCINT0 (3)

/**
 * Code for interrupt: Pin change interrupt request 1.
 */
#define INT_PCINT1 (4)

/**
 * Code for interrupt: Pin change interrupt request 2.
 */
#define INT_PCINT2 (5)

/**
 * Code for interrupt: Watchdog time-out interrupt.
 */
#define INT_WDT (6)

/**
 * Code for interrupt: Timer/counter2 compare match A.
 */
#define INT_TIMER2COMPA (7)

/**
 * Code for interrupt: Timer/counter2 compare match B.
 */
#define INT_TIMER2COMPB (8)

/**
 * Code for interrupt: Timer/counter2 overflow.
 */
#define INT_TIMER2OVF (9)

/**
 * Code for interrupt: Timer/counter1 capture event.
 */
#define INT_TIMER1CAPT (10)

/**
 * Code for interrupt: Timer/counter1 compare match A.
 */
#define INT_TIMER1COMPA (11)

/**
 * Code for interrupt: Timer/counter1 compare match B.
 */
#define INT_TIMER1COMPB (12)

/**
 * Code for interrupt: Timer/counter1 overflow.
 */
#define INT_TIMER1OVF (13)

/**
 * Code for interrupt: Timer/counter0 compare match A.
 */
#define INT_TIMER0COMPA (14)

/**
 * Code for interrupt: Timer/counter0 compare match B.
 */
#define INT_TIMER0COMPB (15)

/**
 * Code for interrupt: SPI serial transfer complete.
 */
#define INT_SPISTC (16)

/**
 * Code for interrupt: USART Rx complete.
 */
#define INT_USARTRX (17)

/**
 * Code for interrupt: USART data register empty.
 */
#define INT_USARTUDRE (18)

/**
 * Code for interrupt: USART Tx complete.
 */
#define INT_USARTTX (19)

/**
 * Code for interrupt: ADC conversion complete.
 */
#define INT_ADC (20)

/**
 * Code for interrupt: EEPROM ready.
 */
#define INT_EEREADY (21)

/**
 * Code for interrupt: Analog comparator.
 */
#define INT_ANALOGCOMP (22)

/**
 * Code for interrupt: 2-wire serial interface.
 */
#define INT_TWI (23)

/**
 * Code for interrupt: Store program memory ready.
 */
#define INT_SPMREADY (24)

#endif /* LZ_SYS_ARCH_AVR_INTERRUPTS_H */

/**
 * @file src/include/Lazuli/sys/arch/AVR/registers.h
 *
 * This file contains general registers definitions for the ATmega 328P.
 */

#ifndef LAZULI_SYS_ARCH_AVR_REGISTERS_H
#define LAZULI_SYS_ARCH_AVR_REGISTERS_H

#include <Lazuli/common.h>

/*
 * General register definitions.
 * Ordered by address value.
 */

/** Port B Input Pins Address              */
#define PINB   DIRECT(0x23)

/** Port B Data Direction Register         */
#define DDRB   DIRECT(0x24)

/** Port B Data Register                   */
#define PORTB  DIRECT(0x25)

/** Port C Input Pins Address              */
#define PINC   DIRECT(0x26)

/** Port C Data Direction Register         */
#define DDRC   DIRECT(0x27)

/** Port C Data Register                   */
#define PORTC  DIRECT(0x28)

/** Port D Input Pins Address              */
#define PIND   DIRECT(0x29)

/** Port D Data Direction Register         */
#define DDRD   DIRECT(0x2a)

/** Port D Data Register                   */
#define PORTD  DIRECT(0x2b)

/** External Interrupt Flag Register       */
#define EIFR   DIRECT(0x3c)

/** External Interrupt Mask Register       */
#define EIMSK  DIRECT(0x3d)

/** Sleep Mode Control Register            */
#define SMCR   DIRECT(0x53)

/** MCU Control Register                   */
#define MCUCR  DIRECT(0x55)

/** Stack Pointer                          */
#define SP     DIRECT_T(0x5d, void*)

/** Power Reduction Register               */
#define PRR    DIRECT(0x64)

/** External Interrupt Control Register A  */
#define EICRA  DIRECT(0x69)


/*
 * Register content definitions.
 * Aliases on bit position for some registers.
 */

/**
 * Content of the PRR register.
 */
enum {
  PRR_PRADC    = POSITION(0), /**< Power Reduction ADC                       */
  PRR_PRUSART0 = POSITION(1), /**< Power Reduction USART0                    */
  PRR_PRSPI    = POSITION(2), /**< Power Reduction Serial Peripheral
                                   Interface                                 */
  PRR_PRTIM1   = POSITION(3), /**< Power Reduction Timer/Counter 1           */
  PRR_PRTIM0   = POSITION(5), /**< Power Reduction Timer/Counter 0           */
  PRR_PRTIM2   = POSITION(6), /**< Power Reduction Timer/Counter 2           */
  PRR_PRTWI    = POSITION(7)  /**< Power Reduction TWI                       */
};

#endif /* LAZULI_SYS_ARCH_AVR_REGISTERS_H */

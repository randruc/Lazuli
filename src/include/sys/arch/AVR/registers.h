/**
 * @file registers.h
 *
 * This file contains general registers definitions for the ATmega 328P.
 */

#ifndef LZ_REGISTERS_H
#define LZ_REGISTERS_H

#include <sys/types.h>

/*
 * General register definitions.
 * Ordered by address value.
 */
#define PINB   DIRECT(0x23)   /** < Port B Input Pins Address               */
#define DDRB   DIRECT(0x24)   /** < Port B Data Direction Register          */
#define PORTB  DIRECT(0x25)   /** < Port B Data Register                    */
#define PINC   DIRECT(0x26)   /** < Port C Input Pins Address               */
#define DDRC   DIRECT(0x27)   /** < Port C Data Direction Register          */
#define PORTC  DIRECT(0x28)   /** < Port C Data Register                    */
#define PIND   DIRECT(0x29)   /** < Port D Input Pins Address               */
#define DDRD   DIRECT(0x2a)   /** < Port D Data Direction Register          */
#define PORTD  DIRECT(0x2b)   /** < Port D Data Register                    */
#define EIFR   DIRECT(0x3c)   /** < External Interrupt Flag Register        */
#define EIMSK  DIRECT(0x3d)   /** < External Interrupt Mask Register        */
#define SMCR   DIRECT(0x53)   /** < Sleep Mode Control Register             */
#define MCUCR  DIRECT(0x55)   /** < MCU Control Register                    */
#define SPL    INDIRECT(0x5d) /** < Stack Pointer Low Register              */
#define SPH    INDIRECT(0x5e) /** < Stack Pointer High Register             */
#define PRR    DIRECT(0x64)   /** < Power Reduction Register                */
#define EICRA  DIRECT(0x69)   /** < External Interrupt Control Register A   */

/*
 * Register content definitions.
 * Aliases on bit position for some registers.
 */

/**
 * Content of the PRR register.
 */
enum {
  PRR_PRADC    = POSITION(0), /** < Power Reduction ADC                       */
  PRR_PRUSART0 = POSITION(1), /** < Power Reduction USART0                    */
  PRR_PRSPI    = POSITION(2), /** < Power Reduction Serial Peripheral
                                    Interface                                 */
  PRR_PRTIM1   = POSITION(3), /** < Power Reduction Timer/Counter 1           */
  PRR_PRTIM0   = POSITION(5), /** < Power Reduction Timer/Counter 0           */
  PRR_PRTIM2   = POSITION(6), /** < Power Reduction Timer/Counter 2           */
  PRR_PRTWI    = POSITION(7)  /** < Power Reduction TWI                       */
};

#endif /* LZ_REGISTERS_H */

/**
 * @file registers.h
 *
 * This file contains machine specific register definitions for the ATmega 328P.
 */

#ifndef LZ_REGISTERS_H
#define LZ_REGISTERS_H

/**
 * Define an indirect read/write register.
 * i.e. A register that is accessed through a pointer.
 *
 * @param X The address of the register.
 */
#define INDIRECT(X) ((char *)X)

/**
 * Define a direct read/write register.
 *
 * @param X The address of the register.
 */
#define DIRECT(X) (*INDIRECT(X))

#define PINB   DIRECT(0x23)   /** < Port B Input Pins Address               */
#define DDRB   DIRECT(0x24)   /** < Port B Data Direction Register          */
#define PORTB  DIRECT(0x25)   /** < Port B Data Register                    */
#define PINC   DIRECT(0x26)   /** < Port C Input Pins Address               */
#define DDRC   DIRECT(0x27)   /** < Port C Data Direction Register          */
#define PORTC  DIRECT(0x28)   /** < Port C Data Register                    */
#define PIND   DIRECT(0x29)   /** < Port D Input Pins Address               */
#define DDRD   DIRECT(0x2a)   /** < Port D Data Direction Register          */
#define PORTD  DIRECT(0x2b)   /** < Port D Data Register                    */
#define TIFR0  DIRECT(0x35)   /** < Timer/Counter 0 Interrupt Flag Register */
#define EIFR   DIRECT(0x3c)   /** < External Interrupt Flag Register        */
#define EIMSK  DIRECT(0x3d)   /** < External Interrupt Mask Register        */
#define TCCR0A DIRECT(0x44)   /** < Timer/Counter Control Register A        */
#define TCCR0B DIRECT(0x45)   /** < Timer/Counter Control Register B        */
#define TCNT0  DIRECT(0x46)   /** < Timer/Counter Register                  */
#define OCR0A  DIRECT(0x47)   /** < Output Compare register A               */
#define OCR0B  DIRECT(0x48)   /** < Output Compare register B               */
#define MCUCR  DIRECT(0x55)   /** < MCU Control Register                    */
#define SPL    INDIRECT(0x5d) /** < Stack Pointer Low Register              */
#define SPH    INDIRECT(0x5e) /** < Stack Pointer High Register             */
#define EICRA  DIRECT(0x69)   /** < External Interrupt Control Register A   */
#define TIMSK0 DIRECT(0x6e)   /** < Timer/Counter Interrupt Mask Register   */

#endif /* LZ_REGISTERS_H */

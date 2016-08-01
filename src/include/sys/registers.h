/**
 * @file registers.h
 *
 * This file contains machine specific register addresses for the ATmega 328P.
 */

#ifndef REGISTERS_H
#define REGISTERS_H

#define PINB  (*((char *)0x23)) /** < Port B Input Pins Address      */
#define DDRB  (*((char *)0x24)) /** < Port B Data Direction Register */
#define PORTB (*((char *)0x25)) /** < Port B Data Register           */
#define PINC  (*((char *)0x26)) /** < Port C Input Pins Address      */
#define DDRC  (*((char *)0x27)) /** < Port C Data Direction Register */
#define PORTC (*((char *)0x28)) /** < Port C Data Register           */
#define PIND  (*((char *)0x29)) /** < Port D Input Pins Address      */
#define DDRD  (*((char *)0x2a)) /** < Port D Data Direction Register */
#define PORTD (*((char *)0x2b)) /** < Port D Data Register           */
#define MCUCR (*((char *)0x55)) /** < MCU Control Register           */
#define SPL   ((char *)0x5d)    /** < Stack Pointer Low Register     */
#define SPH   ((char *)0x5e)    /** < Stack Pointer High Register    */

#endif /* REGISTERS_H */

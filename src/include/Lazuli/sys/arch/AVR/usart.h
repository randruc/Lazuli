/**
 * @file usart.h
 *
 * Describes the interface of the USART driver for the ATMega328p.
 */

#ifndef LZ_AVR_USART_H
#define LZ_AVR_USART_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/*
 * USART Register Definitions.
 * Ordered by address.
 */
#define UCSR0A DIRECT(0xc0) /** < USART Control and Status Register A */
#define UCSR0B DIRECT(0xc1) /** < USART Control and Status Register B */
#define UCSR0C DIRECT(0xc2) /** < USART Control and Status Register C */
#define UBRROL DIRECT(0xc4) /** < USART Baud Rate Register low        */
#define UBRR0H DIRECT(0xc5) /** < USART Baud Rate Register high       */
#define UDR0   DIRECT(0xc6) /** < USART I/O Data Register             */

/**
 * Describes the memory mapping for the USART.
 */
typedef struct {
  volatile u8 ucsr0a;    /** < USART Control and Status Register A */
  volatile u8 ucsr0b;    /** < USART Control and Status Register B */
  volatile u8 ucsr0c;    /** < USART Control and Status Register C */
  volatile u8 _reserved; /*  < Unsed                               */
  volatile u8 ubrr0l;    /** < USART Baud Rate Register low        */
  volatile u8 ubrr0h;    /** < USART Baud Rate Register high       */
  volatile u8 udr0;      /** < USART I/O Data Register             */
}Usart;

/**
 * Content of the UCSR0A register.
 */
enum {
  UCSR0A_RXC0  = POSITION(7), /** < USART Receive Complete             */
  UCSR0A_TXC0  = POSITION(6), /** < USART Transmit Complete            */
  UCSR0A_UDRE0 = POSITION(5), /** < USART Data Register Empty          */
  UCSR0A_FE0   = POSITION(4), /** < Frame Error                        */
  UCSR0A_DOR0  = POSITION(3), /** < Data OverRun                       */
  UCSR0A_UPE0  = POSITION(2), /** < USART Parity Error                 */
  UCSR0A_U2X0  = POSITION(1), /** < Double USART Transmission Speed    */
  UCSR0A_MPCM0 = POSITION(0)  /** < Multi-processor Communication Mode */
};

/**
 * Content of the UCSR0B register.
 */
enum {
  UCSR0B_RXCIE0 = POSITION(7), /** < RX Complete Interrupt Enable         */
  UCSR0B_TXCIE0 = POSITION(6), /** < TX Complete Interrupt Enable         */
  UCSR0B_UDRIE0 = POSITION(5), /** < Data Register Empty Interrupt Enable */
  UCSR0B_RXEN0  = POSITION(4), /** < Receiver Enable                      */
  UCSR0B_TXEN0  = POSITION(3), /** < Transmitter Enable                   */
  UCSR0B_UCSZ02 = POSITION(2), /** < Character Size 2                     */
  UCSR0B_RXB80  = POSITION(1), /** < Receive Data Bit 8                   */
  UCSR0B_TXB80  = POSITION(0)  /** < Transmit Data Bit 8                  */
};

/**
 * Content of the UCSR0C register.
 */
enum {
  UCSR0C_UMSEL01 = POSITION(7), /** < USART Mode Select 1 */
  UCSR0C_UMSEL00 = POSITION(6), /** < USART Mode Select 0 */
  UCSR0C_UPM01   = POSITION(5), /** < Parity Mode 1       */
  UCSR0C_UPM00   = POSITION(4), /** < Parity Mode 0       */
  UCSR0C_USBS0   = POSITION(3), /** < Stop Bit Select     */
  UCSR0C_UCSZ01  = POSITION(2), /** < Character size 1    */
  UCSR0C_UCSZ00  = POSITION(1), /** < Character Size 0    */
  UCSR0C_UCPOL0  = POSITION(0)  /** < Clock Polarity      */
};

/**
 * The default baud rate to use.
 */
#define USART_BAUD_RATE (9600)

/**
 * Initialize the USART.
 */
void
Usart_Init();

/**
 * Transmit a character.
 *
 * @param c The character to transmit.
 */
void
Usart_PutChar(char c);

_EXTERN_C_DECL_END

#endif /* LZ_AVR_USART_H */

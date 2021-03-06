/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief USART API for the ATmega328P.
 * @copyright 2017-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * Describes the interface of the USART driver for the ATMega328p.
 */

#ifndef LAZULI_SYS_ARCH_AVR_USART_H
#define LAZULI_SYS_ARCH_AVR_USART_H

#include <stdint.h>

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/*
 * USART Register Definitions.
 * Ordered by address.
 */

/** USART Control and Status Register A */
#define UCSR0A DIRECT(0xc0)

/** USART Control and Status Register B */
#define UCSR0B DIRECT(0xc1)

/** USART Control and Status Register C */
#define UCSR0C DIRECT(0xc2)

/** USART Baud Rate Register low        */
#define UBRROL DIRECT(0xc4)

/** USART Baud Rate Register high       */
#define UBRR0H DIRECT(0xc5)

/** USART I/O Data Register             */
#define UDR0   DIRECT(0xc6)

/**
 * Describes the memory mapping for the USART.
 */
typedef struct {
  volatile uint8_t ucsr0a;    /**< USART Control and Status Register A */
  volatile uint8_t ucsr0b;    /**< USART Control and Status Register B */
  volatile uint8_t ucsr0c;    /**< USART Control and Status Register C */
  volatile uint8_t _reserved; /**< Unsed                               */
  volatile uint8_t ubrr0l;    /**< USART Baud Rate Register low        */
  volatile uint8_t ubrr0h;    /**< USART Baud Rate Register high       */
  volatile uint8_t udr0;      /**< USART I/O Data Register             */
}Usart;

/**
 * Content of the UCSR0A register.
 */
enum {
  UCSR0A_RXC0  = POSITION(7U), /**< USART Receive Complete             */
  UCSR0A_TXC0  = POSITION(6U), /**< USART Transmit Complete            */
  UCSR0A_UDRE0 = POSITION(5U), /**< USART Data Register Empty          */
  UCSR0A_FE0   = POSITION(4U), /**< Frame Error                        */
  UCSR0A_DOR0  = POSITION(3U), /**< Data OverRun                       */
  UCSR0A_UPE0  = POSITION(2U), /**< USART Parity Error                 */
  UCSR0A_U2X0  = POSITION(1U), /**< Double USART Transmission Speed    */
  UCSR0A_MPCM0 = POSITION(0U)  /**< Multi-processor Communication Mode */
};

/**
 * Content of the UCSR0B register.
 */
enum {
  UCSR0B_RXCIE0 = POSITION(7U), /**< RX Complete Interrupt Enable         */
  UCSR0B_TXCIE0 = POSITION(6U), /**< TX Complete Interrupt Enable         */
  UCSR0B_UDRIE0 = POSITION(5U), /**< Data Register Empty Interrupt Enable */
  UCSR0B_RXEN0  = POSITION(4U), /**< Receiver Enable                      */
  UCSR0B_TXEN0  = POSITION(3U), /**< Transmitter Enable                   */
  UCSR0B_UCSZ02 = POSITION(2U), /**< Character Size 2                     */
  UCSR0B_RXB80  = POSITION(1U), /**< Receive Data Bit 8                   */
  UCSR0B_TXB80  = POSITION(0U)  /**< Transmit Data Bit 8                  */
};

/**
 * Content of the UCSR0C register.
 */
enum {
  UCSR0C_UMSEL01 = POSITION(7U), /**< USART Mode Select 1 */
  UCSR0C_UMSEL00 = POSITION(6U), /**< USART Mode Select 0 */
  UCSR0C_UPM01   = POSITION(5U), /**< Parity Mode 1       */
  UCSR0C_UPM00   = POSITION(4U), /**< Parity Mode 0       */
  UCSR0C_USBS0   = POSITION(3U), /**< Stop Bit Select     */
  UCSR0C_UCSZ01  = POSITION(2U), /**< Character size 1    */
  UCSR0C_UCSZ00  = POSITION(1U), /**< Character Size 0    */
  UCSR0C_UCPOL0  = POSITION(0U)  /**< Clock Polarity      */
};

_EXTERN_C_DECL_END

#endif /* LAZULI_SYS_ARCH_AVR_USART_H */

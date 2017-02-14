/**
 * @file usart.c
 *
 * Describe the driver implementation for the USART of the ATMega328p.
 */

#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/config.h>

/**
 * The baud rate value to set in UBRR0.
 */
#define UBRR0_VALUE ((CLOCK_FREQUENCY / 16 * USART_BAUD_RATE) - 1)

/**
 * A constant pointer to the memory mapped Usart structure.
 */
static Usart * const usart = (Usart*)&UCSR0A;

void
Usart_Init()
{
  /* Set baud rate */
  usart->ubrr0l = 6; /* LO8(UBRR0_VALUE); */
  usart->ubrr0h = 0; /* HI8(UBRR0_VALUE); */

  /* Enable transmitter */
  usart->ucsr0b = UCSR0B_TXEN0;

  /* 8 bits, 1 stop bit, no parity */
  usart->ucsr0c = UCSR0C_UCSZ01 | UCSR0C_UCSZ00;
}

void
Usart_PutChar(char c)
{
  while(!(usart->ucsr0a & UCSR0A_UDRE0));

  usart->udr0 = c;
}

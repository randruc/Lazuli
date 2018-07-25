/**
 * @file src/kern/arch/AVR/usart.c
 *
 * Describe the driver implementation for the USART of the ATMega328p.
 */

#include <stdint.h>

#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/compiler.h>
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
  usart->ubrr0l = 12; /* LO8(UBRR0_VALUE); */
  usart->ubrr0h = 0; /* HI8(UBRR0_VALUE); */

  /* Enable transmitter */
  usart->ucsr0b = UCSR0B_TXEN0;

  /* 8 bits, 1 stop bit, no parity */
  usart->ucsr0c = UCSR0C_UCSZ01 | UCSR0C_UCSZ00;
}

void
Usart_PutChar(char c)
{
  while (!(usart->ucsr0a & UCSR0A_UDRE0));

  usart->udr0 = c;
}

void
Usart_NewLine()
{
  Usart_PutChar('\r');
  Usart_PutChar('\n');
}

/**
 * Array used to get the character representation of a hexadecimal digit
 */
__progmem static const char hexachars[] = {
  '0',
  '1',
  '2',
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  'a',
  'b',
  'c',
  'd',
  'e',
  'f'
};

/**
 * Union to access individual bytes of an integer value (with a maximum width of
 * 32 bits).
 */
typedef union {
  void *pointer;             /**< Holds the pointer value              */
  void (*functionPointer)(); /**< Hold the function pointer value      */
  uint8_t    u8Value;        /**< Holds the 8-bits value               */
  uint16_t   u16Value;       /**< Holds the 16-bits value              */
  uint32_t   u32Value;       /**< Holds the 32-bits value              */
  char  bytes[4];            /**< Easily access each byte of the value */
}IntegerBytes;

/**
 * Print a numerical value stored in an IntegerBytes union to its hexadecimal
 * form, accordig to the correct size.
 *
 * This serves as the base implementation for all specialized
 * Usart_HexaPrint_*() functions.
 *
 * @param integerBytes A pointer to the IntergerBytes union to print.
 * @param size The size in bytes of the value to print.
 */
static void
Usart_HexaPrint_IntegerBytes(IntegerBytes const * const integerBytes,
                             const size_t size)
{
  uint8_t i;
  uint8_t upperPart;
  uint8_t lowerPart;

  Usart_PutChar('0');
  Usart_PutChar('x');

  for (i = size; 0 != i; i--) {
    upperPart = (integerBytes->bytes[i - 1] >> 4) & 0x0f;
    lowerPart = integerBytes->bytes[i - 1] & 0x0f;

    Usart_PutChar(Arch_LoadU8FromProgmem(&hexachars[upperPart]));
    Usart_PutChar(Arch_LoadU8FromProgmem(&hexachars[lowerPart]));
  }
}

void
Usart_HexaPrint_u32(const uint32_t value)
{
  IntegerBytes integerBytes;
  integerBytes.u32Value = value;

  Usart_HexaPrint_IntegerBytes(&integerBytes, sizeof(value));
}

void
Usart_HexaPrint_u16(const uint16_t value)
{
  IntegerBytes integerBytes;
  integerBytes.u16Value = value;

  Usart_HexaPrint_IntegerBytes(&integerBytes, sizeof(value));
}

void
Usart_HexaPrint_u8(const uint8_t value)
{
  IntegerBytes integerBytes;
  integerBytes.u8Value = value;

  Usart_HexaPrint_IntegerBytes(&integerBytes, sizeof(value));
}

void
Usart_HexaPrint_Pointer(void * const pointer)
{
  IntegerBytes integerBytes;
  integerBytes.pointer = pointer;

  Usart_HexaPrint_IntegerBytes(&integerBytes, sizeof(pointer));
}

void
Usart_HexaPrint_FunctionPointer(void (*functionPointer)())
{
  IntegerBytes integerBytes;
  integerBytes.functionPointer = functionPointer;

  Usart_HexaPrint_IntegerBytes(&integerBytes, sizeof(functionPointer));
}

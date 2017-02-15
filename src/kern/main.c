/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <Lazuli/common.h>
#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/linker.h>

void
Int0Handler()
{
}

void
Int1Handler()
{
}

void
Timer0CompareMatchAHandler()
{
}

void
TimerCounter0OverflowHandler()
{
}

/**
 * Main entry point for user tasks.
 */
void
Main()
{
  char c = '4';

  Usart_Init();

  Usart_PutChar('\r');
  Usart_PutChar('\n');

  Usart_PutChar('!');

  Usart_PutChar('\r');
  Usart_PutChar('\n');

  Usart_HexaPrint_u8(0xca);

  Usart_PutChar('\r');
  Usart_PutChar('\n');

  Usart_HexaPrint_u8(15);

  Usart_PutChar('\r');
  Usart_PutChar('\n');

  Usart_HexaPrint_u8(16);

  Usart_PutChar('\r');
  Usart_PutChar('\n');

  Usart_HexaPrint_u16(0xcafe);

  Usart_PutChar('\r');
  Usart_PutChar('\n');

  Usart_HexaPrint_u32(0xcafebabe);

  Usart_PutChar('\r');
  Usart_PutChar('\n');

  Usart_HexaPrint_Pointer(&_ramend);

  Usart_PutChar('\r');
  Usart_PutChar('\n');

  Usart_HexaPrint_Pointer(&_brk);

  Usart_PutChar('\r');
  Usart_PutChar('\n');

  Usart_HexaPrint_Pointer(&c);

  Usart_PutChar('\r');
  Usart_PutChar('\n');

  while(true);
}

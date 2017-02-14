/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <Lazuli/common.h>
#include <Lazuli/sys/arch/AVR/usart.h>

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
  Usart_Init();

  while(true) {
    Usart_PutChar('a');
  }

  while(true);
}

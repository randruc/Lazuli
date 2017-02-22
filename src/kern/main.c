/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <Lazuli/common.h>
#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/linker.h>
#include <Lazuli/sys/scheduler.h>
#include <Lazuli/sys/arch/AVR/registers.h>

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

void
Task1()
{
  u16 i = 0;

  DDRB = (u8)0xff;

  while (true) {
    while(i++ < 20000);
    i = 0;

    PORTB = u8_MIN;

    while(i++ < 20000);
    i = 0;

    PORTB = u8_MAX;
  }
}

/**
 * Main entry point for user tasks.
 */
void
Main()
{
  Usart_Init();
  Usart_PutChar('\r');
  Usart_PutChar('\n');

  Lz_Scheduler_RegisterTask(Task1);
  Lz_Scheduler_Run();

  while(true);
}

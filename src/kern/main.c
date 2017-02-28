/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <Lazuli/common.h>
#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/linker.h>
#include <Lazuli/scheduler.h>
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
TimerCounter0OverflowHandler()
{
}

void
Task2()
{
  char c;

  Usart_Init();

  while (true) {
    for (c = 'a'; c <= 'z'; c++) {
      Usart_PutChar(c);
    }

    Usart_PutChar('\r');
    Usart_PutChar('\n');
  }
}

void
Task1()
{
  u16 i = 0;

  DDRB = (u8)0xff;

  while (true) {
    while (i++ < 20000);
    i = 0;

    PORTB = u8_MIN;

    while (i++ < 20000);
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
  Lz_Scheduler_RegisterTask(Task1);
  Lz_Scheduler_RegisterTask(Task2);

  Lz_Scheduler_Run();

  while (true);
}

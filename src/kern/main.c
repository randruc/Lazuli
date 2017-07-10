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

  while (true) {
    for (c = 'a'; c <= 'z'; c++) {
      Usart_PutChar(c);
    }

    Usart_NewLine();
    while(true); /* Remove that */
  }
}

void
Task1()
{
  u16 i = 0;

  DDRB = (u8)0xff;

  while (true) {
    while (i++ < 60000);
    i = 0;

    PORTB = u8_MIN;

    while (i++ < 60000);
    i = 0;

    PORTB = u8_MAX;
  }
}

void
Task3()
{
  Usart_HexaPrint_Pointer((void * const)SP);
  Usart_NewLine();

  while (true) {
    Usart_PutChar('Y');
    Usart_HexaPrint_FunctionPointer(Task3);
    Usart_NewLine();
    while (1);
  }
}

void
Task4()
{
  while (true) {
    Usart_PutChar('W');
  }
}

void
Task5()
{
  while (true) {
    Usart_PutChar('T');
  }
}

void
Task6()
{
  while (true) {
    Usart_PutChar('F');
  }
}



/**
 * Main entry point for user tasks.
 */
void
Main()
{
  Usart_Init();

  Lz_Scheduler_RegisterTask(Task4);
  Lz_Scheduler_RegisterTask(Task5);
  Lz_Scheduler_RegisterTask(Task6);

  Lz_Scheduler_Run();

  while (true);
}

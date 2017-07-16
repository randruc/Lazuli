/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <Lazuli/common.h>
#include <Lazuli/scheduler.h>

#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/arch/AVR/registers.h>

void
Task1()
{
  u16 i = u16_MAX;

  while (true) {
    Usart_PutChar('W');

    while (i--);
    i = u16_MAX;
  }
}

void
Task2()
{
  u16 i = u16_MAX;

  while (true) {
    Usart_PutChar('T');

    while (i--);
    i = u16_MAX;
  }
}

void
Task3()
{
  while (true) {
    Usart_PutChar('F');
  }
}

void
Task4()
{
  /*
   * EICRA = 0x03;
   * EIMSK = 0x01;
   */

  while (true) {
    Lz_WaitInt0();
    Usart_PutChar('0');
  }
}

void
Task5()
{
  /*
   * EICRA = 0x03;
   * EIMSK = 0x01;
   */

  while (true) {
    Lz_WaitInt0();
    Usart_PutChar('1');
  }
}

/**
 * Main entry point for user tasks.
 */
void
Main()
{
  Usart_Init();

  Lz_Scheduler_RegisterTask(Task1);
  Lz_Scheduler_RegisterTask(Task2);
  Lz_Scheduler_RegisterTask(Task4);
  Lz_Scheduler_RegisterTask(Task5);

  EICRA = 0x03;
  EIMSK = 0x01;

  Lz_Scheduler_Run();

  while (true);
}

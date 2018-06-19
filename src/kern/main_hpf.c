/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/arch/AVR/interrupts.h>

void
Task1()
{
  u16 i = u16_MAX >> 1;

  while (true) {
    Usart_PutChar('.');

    while (i--);
    i = u16_MAX;
  }
}

void
Task2()
{
  u16 i = u16_MAX >> 1;

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
  volatile u16 i;
  char c;

  while (true) {
    Lz_WaitInterrupt(INT_INT0);

    for (c = 'a'; c <= 'z'; c++) {
      Usart_PutChar(c);
      i = u16_MAX >> 1;
      while (i--);
    }
  }
}

void
Task5()
{
  volatile u16 i;
  char c;

  while (true) {
    Lz_WaitInterrupt(INT_INT1);

    for (c = 'A'; c <= 'Z'; c++) {
      Usart_PutChar(c);
      i = u16_MAX >> 1;
      while (i--);
    }
  }
}

/**
 * Main entry point for user tasks.
 */
int
main()
{
  Lz_TaskConfiguration taskConfiguration;

  Usart_Init();

  Lz_SetSchedulerClass(LZ_SCHED_HPF);

  Lz_InitTaskConfiguration(&taskConfiguration);
  taskConfiguration.priority = 5;
  Lz_RegisterTask(Task1, &taskConfiguration);

  Lz_InitTaskConfiguration(&taskConfiguration);
  taskConfiguration.priority = 10;
  Lz_RegisterTask(Task4, &taskConfiguration);

  Lz_InitTaskConfiguration(&taskConfiguration);
  taskConfiguration.priority = 15;
  Lz_RegisterTask(Task5, &taskConfiguration);

  EICRA = 0x0f;
  EIMSK = 0x03;

  Lz_Run();

  while (true);
}

/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/arch/AVR/usart.h>

void
Task1()
{
  uint16_t i = UINT16_MAX >> 1;

  while (true) {
    Usart_PutChar('.');

    while (i--);

    i = UINT16_MAX;
  }
}

void
Task2()
{
  uint16_t i = UINT16_MAX >> 1;

  while (true) {
    Usart_PutChar('T');

    while (i--);

    i = UINT16_MAX;
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
  volatile uint16_t i;
  char c;

  while (true) {
    Lz_WaitInterrupt(INT_INT0);

    for (c = 'a'; c <= 'j'; c++) {
      Usart_PutChar(c);
      i = UINT16_MAX >> 1;

      while (i--);
    }
  }
}

void
Task5()
{
  volatile uint16_t i;
  char c;

  while (true) {
    Lz_WaitInterrupt(INT_INT1);

    for (c = 'A'; c <= 'J'; c++) {
      Usart_PutChar(c);
      i = UINT16_MAX >> 1;

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

  /*
   * Lz_InitTaskConfiguration(&taskConfiguration);
   * taskConfiguration.priority = 5;
   * Lz_RegisterTask(Task1, &taskConfiguration);
   */

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

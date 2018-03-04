/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

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
  char const *c;

  while (true) {
    Lz_WaitInt0();

    c = Lz_GetTaskName();

    while (NULL != c && '\0' != *c) {
      Usart_PutChar(*c);
      c++;
    }
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
  Lz_TaskConfiguration taskConfiguration;

  Usart_Init();

  Lz_InitTaskConfiguration(&taskConfiguration);
  taskConfiguration.name = NAME_OF(Task1);
  Lz_RegisterTask(Task1, &taskConfiguration);

  Lz_InitTaskConfiguration(&taskConfiguration);
  taskConfiguration.name = NAME_OF(Task2);
  Lz_RegisterTask(Task2, &taskConfiguration);

  Lz_InitTaskConfiguration(&taskConfiguration);
  taskConfiguration.name = NAME_OF(Task4);
  Lz_RegisterTask(Task4, &taskConfiguration);

  Lz_InitTaskConfiguration(&taskConfiguration);
  taskConfiguration.name = NAME_OF(Task5);
  Lz_RegisterTask(Task5, &taskConfiguration);

  EICRA = 0x03;
  EIMSK = 0x01;

  Lz_Run();

  while (true);
}

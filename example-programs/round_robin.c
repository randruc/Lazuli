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
  /* volatile */ uint16_t i = UINT16_MAX >> 1;

  for (;;) {
    Usart_PutChar('W');

    while (i--);

    i = UINT16_MAX;
  }
}

void
Task2()
{
  /* volatile */ uint16_t i = UINT16_MAX >> 1;

  for (;;) {
    Usart_PutChar('T');

    while (i--);

    i = UINT16_MAX;
  }
}

void
Task3()
{
  for (;;) {
    Usart_PutChar('F');
  }
}

void
Task4()
{
  char const *c;

  for (;;) {
    Lz_WaitInterrupt(INT_INT0);

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
  char const *c;

  for (;;) {
    Lz_WaitInterrupt(INT_INT0);

    c = Lz_GetTaskName();

    while (NULL != c && '\0' != *c) {
      Usart_PutChar(*c);
      c++;
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

  /* Lz_SetSchedulerClass(LZ_SCHED_RR); */

  Lz_RegisterTask(Task1, NULL);
  Lz_RegisterTask(Task2, NULL);

  Lz_InitTaskConfiguration(&taskConfiguration);
  taskConfiguration.name = NAME_OF(Task4);
  Lz_RegisterTask(Task4, &taskConfiguration);

  Lz_InitTaskConfiguration(&taskConfiguration);
  taskConfiguration.name = NAME_OF(Task5);
  Lz_RegisterTask(Task5, &taskConfiguration);

  EICRA = 0x03;
  EIMSK = 0x01;

  Lz_Run();

  for (;;);
}

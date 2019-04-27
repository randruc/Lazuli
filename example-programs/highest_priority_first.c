/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/serial.h>

#include <Lazuli/sys/arch/AVR/interrupts.h>
#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/arch/AVR/usart.h>

void
Task1(void)
{
  uint16_t i = UINT16_MAX >> 1;

  for (;;) {
    Usart_PutChar('.');

    while (i--);

    i = UINT16_MAX;
  }
}

void
Task2(void)
{
  uint16_t i = UINT16_MAX >> 1;

  for (;;) {
    Usart_PutChar('T');

    while (i--);

    i = UINT16_MAX;
  }
}

void
Task3(void)
{
  for (;;) {
    Usart_PutChar('F');
  }
}

void
Task4(void)
{
  volatile uint16_t i;
  char c;

  for (;;) {
    Lz_WaitInterrupt(INT_INT0);

    for (c = 'a'; c <= 'j'; ++c) {
      Usart_PutChar(c);
      i = UINT16_MAX >> 1;

      while (i--);
    }
  }
}

void
Task5(void)
{
  volatile uint16_t i;
  char c;

  for (;;) {
    Lz_WaitInterrupt(INT_INT1);

    for (c = 'A'; c <= 'J'; ++c) {
      Usart_PutChar(c);
      i = UINT16_MAX >> 1;

      while (i--);
    }
  }
}

static void
EnableSerialTransmission() {
  Lz_SerialConfiguration serialConfiguration;

  Lz_Serial_GetConfiguration(&serialConfiguration);
  serialConfiguration.enableFlags = LZ_SERIAL_ENABLE_TRANSMIT;
  Lz_Serial_SetConfiguration(&serialConfiguration);
}

/**
 * Main entry point for user tasks.
 */
void
main(void)
{
  Lz_TaskConfiguration taskConfiguration;

  EnableSerialTransmission();

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

  for (;;);
}

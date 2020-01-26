/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/serial.h>
#include <Lazuli/clock_24.h>

#include <Lazuli/sys/arch/AVR/usart.h>

void
Task1()
{
  volatile uint8_t i = 0;

  for (;;) {
    i++;
  }
}

static void
EnableSerialTransmission() {
  Lz_SerialConfiguration serialConfiguration;

  Lz_Serial_GetConfiguration(&serialConfiguration);
  serialConfiguration.enableFlags = LZ_SERIAL_ENABLE_TRANSMIT;
  serialConfiguration.speed = LZ_SERIAL_SPEED_19200;
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

  Usart_PrintRawString(".......\r\n");

  Lz_SetSchedulerClass(LZ_SCHED_RMS);

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.period = 4;
  taskConfiguration.completion = 1;
  taskConfiguration.name = "1";
  Lz_RegisterTask(Task1, &taskConfiguration);

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.period = 6;
  taskConfiguration.completion = 2;
  taskConfiguration.name = "2";
  Lz_RegisterTask(Task1, &taskConfiguration);

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.period = 12;
  taskConfiguration.completion = 3;
  taskConfiguration.name = "3";
  Lz_RegisterTask(Task1, &taskConfiguration);

  Usart_PrintRawString("------\r\n");

  Lz_Run();

  for (;;);
}

/**
 * @file main.c
 *
 * Entry point for user tasks.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/serial.h>

void
Task1()
{
  volatile uint16_t i = 0;

  for (;;) {
    i++;
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

  Lz_TaskConfiguration_Init(&taskConfiguration);

  taskConfiguration.period = 4;
  taskConfiguration.completion = 6;
  
  EnableSerialTransmission();
  
  Lz_SetSchedulerClass(LZ_SCHED_RMS);

  Lz_RegisterTask(Task1, &taskConfiguration);

  Lz_Run();

  for (;;);
}

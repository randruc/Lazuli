/**
 * @file clock24.c
 *
 * An example program demonstrating a simple real-time task: a clock.
 */

#include <stdio.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/clock_24.h>
#include <Lazuli/serial.h>

DEPENDENCY_ON_MODULE(CLOCK_24);
DEPENDENCY_ON_MODULE(PRINTF);
DEPENDENCY_ON_MODULE(SERIAL);

/**
 * Main clock task.
 */
void
ClockTask(void)
{
  Clock24 clock24;

  for (;;) {
    Lz_Task_WaitActivation();

    Lz_Clock24_Get(&clock24);

    printf("%02u:%02u:%02u\r\n",
           clock24.hours,
           clock24.minutes,
           clock24.seconds);
  }
}

/**
 * Main entry point for user tasks.
 */
int
main(void)
{
  Lz_TaskConfiguration taskConfiguration;
  Lz_SerialConfiguration serialConfiguration;

  /*
   * Enable serial tansmission.
   */
  Lz_Serial_GetConfiguration(&serialConfiguration);
  serialConfiguration.enableFlags = LZ_SERIAL_ENABLE_TRANSMIT;
  serialConfiguration.speed = LZ_SERIAL_SPEED_19200;
  Lz_Serial_SetConfiguration(&serialConfiguration);

  /*
   * Configure scheduling parameters of the clock task.
   */
  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.schedulingPolicy = CYCLIC_RT;
  taskConfiguration.period = 50;
  taskConfiguration.completion = 25;
  Lz_RegisterTask(ClockTask, &taskConfiguration);

  Lz_Run();
}

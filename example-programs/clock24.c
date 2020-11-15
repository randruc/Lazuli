/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief A simple clock.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * An example program demonstrating a simple real-time task: a clock.
 */

#include <stdio.h>

#include <Lazuli/clock_24.h>
#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
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

    printf("%02u:%02u:%02u" LZ_CONFIG_SERIAL_NEWLINE,
           clock24.hours,
           clock24.minutes,
           clock24.seconds);
  }
}

/**
 * Main entry point for user tasks.
 */
void
main(void)
{
  Lz_TaskConfiguration taskConfiguration;
  Lz_SerialConfiguration serialConfiguration;

  /*
   * Enable serial transmission.
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

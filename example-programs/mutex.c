/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Mutexes demonstration program.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * Example program to demonstrate the use of mutexes in Lazuli.
 */

#include <stdint.h>
#include <stdio.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/mutex.h>
#include <Lazuli/serial.h>

DEPENDENCY_ON_MODULE(SERIAL);
DEPENDENCY_ON_MODULE(MUTEX);

static Lz_Mutex mutex = LZ_MUTEX_INIT;

void
Task(void)
{
  Lz_Mutex_Lock(&mutex);
  puts(Lz_Task_GetName());
  Lz_Mutex_Unlock(&mutex);
}

static void
EnableSerialTransmission(void) {
  Lz_SerialConfiguration serialConfiguration;

  Lz_Serial_GetConfiguration(&serialConfiguration);
  serialConfiguration.enableFlags = LZ_SERIAL_ENABLE_TRANSMIT;
  serialConfiguration.speed = LZ_SERIAL_SPEED_19200;
  Lz_Serial_SetConfiguration(&serialConfiguration);
}

void
main(void)
{
  Lz_TaskConfiguration taskConfiguration;

  EnableSerialTransmission();

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.name =
    "Task 1 says: "
    "\"An ounce of prevention is better than a pound of cure.\""
    LZ_CONFIG_SERIAL_NEWLINE;
  Lz_RegisterTask(Task, &taskConfiguration);

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.name =
    "Task 2 says: "
    "\"Don't count your chickens before they hatch.\"" LZ_CONFIG_SERIAL_NEWLINE;
  Lz_RegisterTask(Task, &taskConfiguration);

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.name =
    "Task 3 says: "
    "\"Beauty is in the eye of the beholder.\"" LZ_CONFIG_SERIAL_NEWLINE;
  Lz_RegisterTask(Task, &taskConfiguration);

  Lz_TaskConfiguration_Init(&taskConfiguration);
  taskConfiguration.name =
    "Task 4 says: "
    "\"Fire is a good servant but a bad master.\"" LZ_CONFIG_SERIAL_NEWLINE;
  Lz_RegisterTask(Task, &taskConfiguration);

  puts(LZ_CONFIG_SERIAL_NEWLINE);

  Lz_Run();
}

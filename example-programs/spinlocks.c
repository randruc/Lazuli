/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Spinlocks demonstration program.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * Example program to demonstrate the use of spinlocks in Lazuli.
 */

#include <stdint.h>
#include <stdio.h>

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>
#include <Lazuli/serial.h>
#include <Lazuli/spinlock.h>

DEPENDENCY_ON_MODULE(SPINLOCK);
DEPENDENCY_ON_MODULE(SERIAL);

static Lz_Spinlock lock = LZ_SPINLOCK_INIT;

void
Task(void)
{
  Lz_Spinlock_Lock(&lock);
  puts(Lz_Task_GetName());
  Lz_Spinlock_Unlock(&lock);
}

static void
EnableSerialTransmission(void) {
  Lz_SerialConfiguration serialConfiguration;

  Lz_Serial_GetConfiguration(&serialConfiguration);
  serialConfiguration.enableFlags = LZ_SERIAL_ENABLE_TRANSMIT;
  serialConfiguration.speed = LZ_SERIAL_SPEED_19200;
  Lz_Serial_SetConfiguration(&serialConfiguration);
}

int
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

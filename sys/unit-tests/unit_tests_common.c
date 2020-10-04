/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Lazuli kernel unit tests micro framework.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file contains the common utility definitions for the Lazuli kernel unit
 * tests suite.
 */

#include "unit_tests_common.h"

#include <stdint.h>
#include <stdio.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>
#include <Lazuli/serial.h>

DEPENDENCY_ON_MODULE(SERIAL);

void
Assert(const bool cond, const uint16_t line)
{
  if (!cond) {
    printf("0x%x" LZ_CONFIG_SERIAL_NEWLINE, line);
  }
}

bool
StringsAreEqual(const char * const s1, const char * const s2)
{
  size_t i = 0;

  while ('\0' != s1[i]) {
    if (s1[i] != s2[i]) {
      return false;
    }

    ++i;
  }

  if ('\0' != s2[i]) {
    return false;
  }

  return true;
}

/**
 * Activate serial transmission.
 */
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
  EnableSerialTransmission();

  puts(LZ_CONFIG_SERIAL_NEWLINE
       "--BEGIN tests:0123456789!"
       LZ_CONFIG_SERIAL_NEWLINE);

  ExecuteTests();

  puts("." LZ_CONFIG_SERIAL_NEWLINE);

  for (;;);
}

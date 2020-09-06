/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Serial port configuration implementation.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the implementation of serial interface configuration.
 */

#include <Lazuli/common.h>
#include <Lazuli/config.h>
#include <Lazuli/serial.h>
#include <Lazuli/sys/arch/arch.h>

void
Lz_Serial_GetConfiguration(Lz_SerialConfiguration * const serialConfiguration)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_SERIAL) {
    if (NULL == serialConfiguration) {
      return;
    }
  }

  Arch_GetSerialConfiguration(serialConfiguration);
}

void
Lz_Serial_SetConfiguration(Lz_SerialConfiguration * const serialConfiguration)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_SERIAL) {
    if (NULL == serialConfiguration) {
      return;
    }
  }

  Arch_SetSerialConfiguration(serialConfiguration);
}

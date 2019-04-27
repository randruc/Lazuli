/**
 * @file src/kern/serial.c
 * @brief Serial port configuration implementation.
 * @date Mar 2019
 * @author Remi Andruccioli
 *
 * This file describes the implementation of serial interface configuration.
 */

#include <Lazuli/common.h>
#include <Lazuli/serial.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/config.h>

void
Lz_Serial_GetConfiguration(Lz_SerialConfiguration * const serialConfiguration)
{
  if (CONFIG_CHECK_NULL_PARAMETERS_IN_SERIAL) {
    if (NULL == serialConfiguration) {
      return;
    }
  }

  Arch_GetSerialConfiguration(serialConfiguration);
}

void
Lz_Serial_SetConfiguration(Lz_SerialConfiguration * const serialConfiguration)
{
  if (CONFIG_CHECK_NULL_PARAMETERS_IN_SERIAL) {
    if (NULL == serialConfiguration) {
      return;
    }
  }

  Arch_SetSerialConfiguration(serialConfiguration);
}
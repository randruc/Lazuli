/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Hardware specific layer.
 * @copyright 2018-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the implementation of some architecture-specific
 * functions.
 * For now it's typically what can be written in pure C, so what doesn't go to
 * startup.S.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>
#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/arch/arch.h>

#include "avr_arch.h"

void
Arch_InitIdleCpuMode(void)
{
#if LZ_CONFIG_ON_IDLE_SLEEP &&                                          \
  ((LZ_CONFIG_AVR_SLEEP_WITH_IDLE_MODE +                                    \
    LZ_CONFIG_AVR_SLEEP_WITH_ADC_NOISE_REDUCTION_MODE +                     \
    LZ_CONFIG_AVR_SLEEP_WITH_POWER_DOWN_MODE +                              \
    LZ_CONFIG_AVR_SLEEP_WITH_POWER_SAVE_MODE +                              \
    LZ_CONFIG_AVR_SLEEP_WITH_STANDBY_MODE +                                 \
    LZ_CONFIG_AVR_SLEEP_WITH_EXTENDED_STANDBY_MODE) > 1)
#error "Idle mode for architecture AVR must be unique."
#endif

  if (!LZ_CONFIG_ON_IDLE_SLEEP) {
    return;
  }

  if (LZ_CONFIG_AVR_SLEEP_WITH_ADC_NOISE_REDUCTION_MODE) {
    SMCR = 0x02;
  } else if (LZ_CONFIG_AVR_SLEEP_WITH_POWER_DOWN_MODE) {
    SMCR = 0x04;
  } else if (LZ_CONFIG_AVR_SLEEP_WITH_POWER_SAVE_MODE) {
    SMCR = 0x06;
  } else if (LZ_CONFIG_AVR_SLEEP_WITH_STANDBY_MODE) {
    SMCR = 0x0c;
  } else if (LZ_CONFIG_AVR_SLEEP_WITH_EXTENDED_STANDBY_MODE) {
    SMCR = 0x0e;
  } else {
    /* Default value : LZ_CONFIG_AVR_SLEEP_WITH_IDLE_MODE */
    SMCR = 0x00;
  }
}

void
Arch_CpuSleep(void)
{
  SET_BITS(SMCR, uint8_t, POSITION(0U));
  cpu_sleep();
  CLEAR_BITS(SMCR, uint8_t, POSITION(0U));
}

/* TODO: Put that in a module */
void
Arch_InitInstrumentation(void)
{
  if (!LZ_CONFIG_INSTRUMENT_CONTEXT_SWITCHES) {
    return;
  }

  /* Direction of the pin */
  /* WARNING: This is very specific to the AVR ATmega328p. */
  SET_BITS(DIRECT(LZ_CONFIG_AVR_INSTRUMENT_PORT - 1),
           uint8_t,
           POSITION(LZ_CONFIG_AVR_INSTRUMENT_POSITION));
  CLEAR_BITS(DIRECT(LZ_CONFIG_AVR_INSTRUMENT_PORT),
             uint8_t,
             POSITION(LZ_CONFIG_AVR_INSTRUMENT_POSITION));
}

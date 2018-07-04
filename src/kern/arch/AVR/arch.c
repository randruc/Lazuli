/**
 * @file src/kern/arch/AVR/arch.c
 * @brief Hardware specific layer.
 * @date Jul 2018
 * @author Remi Andruccioli
 *
 * This file describes the implementation of some architecture-specific
 * functions.
 */

#include <Lazuli/common.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/arch/AVR/registers.h>
#include <Lazuli/sys/config.h>

#include "avr_arch.h"

/**
 * Initialize all parameters for idle CPU.
 */
static void
InitIdleMode()
{
#if ON_IDLE_SLEEP && ((SLEEP_WITH_IDLE_MODE +                    \
                       SLEEP_WITH_ADC_NOISE_REDUCTION_MODE +     \
                       SLEEP_WITH_POWER_DOWN_MODE +              \
                       SLEEP_WITH_POWER_SAVE_MODE +              \
                       SLEEP_WITH_STANDBY_MODE +                 \
                       SLEEP_WITH_EXTENDED_STANDBY_MODE) > 1)
#error Idle mode for architecture AVR must be unique.
#endif

  if (!ON_IDLE_SLEEP) {
    return;
  }

  if (SLEEP_WITH_ADC_NOISE_REDUCTION_MODE) {
    SMCR = 0x02;
  } else if (SLEEP_WITH_POWER_DOWN_MODE) {
    SMCR = 0x04;
  } else if (SLEEP_WITH_POWER_SAVE_MODE) {
    SMCR = 0x06;
  } else if (SLEEP_WITH_STANDBY_MODE) {
    SMCR = 0x0c;
  } else if (SLEEP_WITH_EXTENDED_STANDBY_MODE) {
    SMCR = 0x0e;
  } else {
    /* Default value : SLEEP_WITH_IDLE_MODE */
    SMCR = 0x00;
  }
}

void
Arch_Init()
{
  InitIdleMode();
}

void
Arch_CpuSleep()
{
  SET_BITS(SMCR, u8, POSITION(0));
  cpu_sleep();
  CLEAR_BITS(SMCR, u8, POSITION(0));
}

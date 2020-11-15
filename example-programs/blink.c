/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief A simple LED blinking task.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file demonstrates a single task which is an LED bliking with a period of
 * 1 second.
 */

#include <stdint.h>

#include <Lazuli/lazuli.h>
#include <Lazuli/sys/arch/AVR/registers.h>

/*
 * This is the Blink task. It simply blinks the built-in LED on Arduino
 * platforms.
 * This task is scheduled in real-time. It is configured to blink with an exact
 * period of 1 second.
 */
void
Blink()
{
  /* On the Arduino, this pin corresponds to the built-in LED */
  const uint8_t ledPin = 0x20;

  DDRB |= ledPin;   /* Set the pin to be an output pin        */
  PORTB &= ~ledPin; /* The initial state of the pin will be 0 */

  /* Now this is the main loop of this task */
  for (;;) {
    /* Wait for the next real-time activation of the task */
    Lz_Task_WaitActivation();

    PINB |= ledPin; /* Toggle the pin */
  }
}

void
main(void)
{
  /* Allocate a configuration object on the stack                      */
  Lz_TaskConfiguration configuration;

  /* Initialize the configuration object with default values           */
  Lz_TaskConfiguration_Init(&configuration);

  /* Configure the Blink task to be cyclic real-time (RMS scheduling)  */
  configuration.schedulingPolicy = CYCLIC_RT;

  /* The Blink task has a period of 25 time slices.                    */
  /* Our platform has a 16 MHz clock, and the system clock resolution  */
  /* frequency is configured to 50 Hz. This is an arbitrary value that */
  /* can be configured by the user.                                    */
  /* With a system clock resolution frequency set to 50 Hz, the system */
  /* clock period is then 1 / 50 = 0.02 second.                        */
  /* So 0.02 * 25 = 0.5 second, which corresponds to the half period   */
  /* of our task.                                                      */
  configuration.period = 25;

  /* The Blink task has a completion of 10 time slices (arbitrary here */
  /* because our task does almost nothing).                            */
  configuration.completion = 10;

  /* Register the Blink task to run with the parameters above          */
  Lz_RegisterTask(Blink, &configuration);

  /* Run the system                                                    */
  Lz_Run();
}

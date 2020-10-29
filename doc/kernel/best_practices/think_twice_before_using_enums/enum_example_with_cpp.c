/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 */

typedef unsigned char planet_t;

#define MERCURY ((planet_t)0xa1)
#define VENUS ((planet_t)0xb2)
#define EARTH ((planet_t)0xc3)
#define MARS ((planet_t)0xd4)

volatile planet_t planet;

void
example(void)
{
  planet = EARTH;
}

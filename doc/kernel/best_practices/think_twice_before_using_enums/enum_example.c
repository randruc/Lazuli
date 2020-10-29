/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 */

enum Planet {
  MERCURY = 0xa1,
  VENUS = 0xb2,
  EARTH = 0xc3,
  MARS = 0xd4
};

volatile enum Planet planet;

void
example(void)
{
  planet = EARTH;
}

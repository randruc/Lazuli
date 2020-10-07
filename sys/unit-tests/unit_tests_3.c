/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Lazuli kernel unit tests part 3.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file contains unit tests to test string related functions.
 */

#include "unit_tests_common.h"

#include <string.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>

DEPENDENCY_ON_MODULE(STRING);

UNIT_TEST(Strlen_1)
{
  ASSERT(4 == strlen("test"));
}

UNIT_TEST(Strlen_2)
{
  ASSERT(0 == strlen(NULL));
}

UNIT_TEST(Strlen_3)
{
  ASSERT(0 == strlen(""));
}

UNIT_TEST(Strlen_4)
{
  ASSERT(1 == strlen("."));
}

UNIT_TEST(Strlen_5)
{
  ASSERT(5 == strlen("12\n34"));
}

UNIT_TEST(Strlen_6)
{
  ASSERT(6 == strlen("12\r\n34"));
}

void
ExecuteTests(void)
{
  Strlen_1();
  Strlen_2();
  Strlen_3();
  Strlen_4();
  Strlen_5();
  Strlen_6();
}

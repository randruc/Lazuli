/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Lazuli kernel unit tests micro framework API.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file contains the common utility declarations for the Lazuli kernel unit
 * tests suite.
 */

#ifndef UNIT_TESTS_COMMON_H
#define UNIT_TESTS_COMMON_H

#include <stdint.h>

#include <Lazuli/common.h>

/**
 * Execute all unit tests.
 */
void
ExecuteTests(void);

/**
 * Perform an assertion inside a unit test.
 *
 * If this assertion is false the corresponding line number will be printed on
 * serial line.
 *
 * @param C The assertion to perform.
 */
#define ASSERT(C) Assert(C, __LINE__)

/**
 * Declare a unit test.
 *
 * @param N The name of the test to declare.
 */
#define UNIT_TEST(N) static void N(void)

/**
 * Internal function used to perform an assertion.
 *
 * @param cond The condition to test.
 * @param line The line number of the assertion.
 */
void
Assert(const bool cond, const uint16_t line);

/**
 * Test if two strings are equal.
 *
 * @param s1 The first string to test.
 * @param s2 The second string to test.
 *
 * @return true if the 2 strings are the same, else false.
 */
bool
StringsAreEqual(const char * const s1, const char * const s2);

#endif /* UNIT_TESTS_COMMON_H */

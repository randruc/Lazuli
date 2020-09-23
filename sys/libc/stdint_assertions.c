/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Static assertions on the width of types defined in stdint.
 * @copyright 2018-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file performs static assertions to verify the validity of the
 * definitions of file stdint.h.
 */

#include <stdint.h>

#include <Lazuli/common.h>

STATIC_ASSERT(sizeof(int8_t) == 1,  int8_t_MUST_be_1_byte_long);
STATIC_ASSERT(sizeof(int16_t) == 2, int16_t_MUST_be_2_byte_long);
STATIC_ASSERT(sizeof(int32_t) == 4, int32_t_MUST_be_4_bytes_long);

STATIC_ASSERT(sizeof(int_fast8_t) >= 1,
              int_fast8_t_MUST_be_at_least_1_bytes_long);
STATIC_ASSERT(sizeof(int_fast16_t) >= 2,
              int_fast16_t_MUST_be_at_least_2_bytes_long);
STATIC_ASSERT(sizeof(int_fast32_t) >= 4,
              int_fast32_t_MUST_be_at_least_4_bytes_long);

STATIC_ASSERT(sizeof(int_least8_t) >= 1,
              int_least8_t_MUST_be_at_least_1_bytes_long);
STATIC_ASSERT(sizeof(int_least16_t) >= 2,
              int_least16_t_MUST_be_at_least_2_bytes_long);
STATIC_ASSERT(sizeof(int_least32_t) >= 4,
              int_least32_t_MUST_be_at_least_4_bytes_long);

STATIC_ASSERT(sizeof(intptr_t) >= sizeof(void*),
              intptr_t_MUST_be_capable_of_holding_a_pointer);

STATIC_ASSERT(sizeof(uint8_t) == 1,  uint8_t_MUST_be_1_byte_long);
STATIC_ASSERT(sizeof(uint16_t) == 2, uint16_t_MUST_be_2_byte_long);
STATIC_ASSERT(sizeof(uint32_t) == 4, uint32_t_MUST_be_4_bytes_long);

STATIC_ASSERT(sizeof(uint_fast8_t) >= 1,
              uint_fast8_t_MUST_be_at_least_1_bytes_long);
STATIC_ASSERT(sizeof(uint_fast16_t) >= 2,
              uint_fast16_t_MUST_be_at_least_2_bytes_long);
STATIC_ASSERT(sizeof(uint_fast32_t) >= 4,
              uint_fast32_t_MUST_be_at_least_4_bytes_long);

STATIC_ASSERT(sizeof(uint_least8_t) >= 1,
              uint_least8_t_MUST_be_at_least_1_bytes_long);
STATIC_ASSERT(sizeof(uint_least16_t) >= 2,
              uint_least16_t_MUST_be_at_least_2_bytes_long);
STATIC_ASSERT(sizeof(uint_least32_t) >= 4,
              uint_least32_t_MUST_be_at_least_4_bytes_long);

STATIC_ASSERT(sizeof(uintptr_t) >= sizeof(void*),
              uintptr_t_MUST_be_capable_of_holding_a_pointer);

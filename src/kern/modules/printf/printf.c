/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file src/kern/modules/printf/printf.c
 * @brief printf implementation.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the implementation of printf.
 * For now it implements a very minimal subset of stdlib's printf().
 * For now, this module is useless if module SERIAL is not used.
 */

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>

#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/printf.h>

/** @cond false */
STATIC_ASSERT(2 == sizeof(unsigned int),
              Sizeof_unsigned_int_not_supported_for_printf);
/** @endcond */

/*
 * Warning! The stack usage of this function is important! Reduce the stack
 * to its strict minimum.
 */
int
printf(const char *format, ...)
{
  va_list args;
  int total = 0;
  const char *c = format;

  if (NULL == format) {
    return 0;
  }

  va_start(args, format);

  for (; '\0' != *c; ++c) {
    if ('%' == *c) {
      bool padWithZeros = false;
      bool firstDigitPassed = false;
      uint8_t padLength = 0;

      for (++c; '\0' != *c; ++c) {
        if ('d' == *c || 'i' == *c || 'u' == *c) {
          unsigned int absolute;
          bool isNegative;
          char buffer[5];
          uint8_t size;

          if ('u' == *c) {
            isNegative = false;
            absolute = va_arg(args, unsigned int);
          } else {
            const int value = va_arg(args, int);
            isNegative = value < 0;
            absolute = ABS(value);

            if (isNegative) {
              Usart_PutChar('-');
              ++total;
            }
          }

          if (2 == sizeof(unsigned int)) {
            size = Printf_ConvertU16ToDecimal(absolute, buffer);
          } else {
            /*
             * Add here the support for another sizeof(unsigned int).
             *
             * Remember to change the size of 'buffer', and add the new
             * sizeof(unsigned int) in the STATIC_ASSERT.
             */
          }

          total += size;

          if (padWithZeros && padLength > size) {
            padLength -= size;

            if (isNegative && padLength > 0) {
              --padLength;
            }

            total += padLength;

            while (padLength-- > 0) {
              Usart_PutChar('0');
            }
          }

          while (size-- > 0) {
            Usart_PutChar(buffer[size]);
          }

          break;
        } else if ('0' == *c && !firstDigitPassed) {
          /* This test MUST be performed before the next one */
          padWithZeros = true;
          firstDigitPassed = true;
        } else if (*c >= '0' && *c <= '9') {
          padLength = padLength * 10 + *c - '0';
          firstDigitPassed = true;
        } else if ('%' == *c) {
          ++total;
          Usart_PutChar('%');

          break;
        } else {
          /*
           * TODO: Error
           */
        }
      }
    } else {
      ++total;
      Usart_PutChar(*c);
    }
  }

  va_end(args);

  return total;
}

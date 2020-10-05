/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
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

#include <Lazuli/sys/printf.h>

/** @cond false */
STATIC_ASSERT(2 == sizeof(unsigned int),
              Sizeof_unsigned_int_not_supported_for_printf);
/** @endcond */

/**
 * Convert a base 16 unit value to its hexadecimal digit representation.
 * The value must be <= 15.
 *
 * @param i The value to convert.
 * @param isUpper A boolean value indicating if the conversion must be done in
 *        uppercase.
 *
 * @return The hexadecimal character representing the value.
 */
static char
GetHexDigit(const uint8_t i, const bool isUpper)
{
  /* We assume that i <= 15 */

  if (i <= 9) {
    return '0' + i;
  }

  if (isUpper) {
    return 'A' + i - 10;
  }

  return 'a' + i - 10;
}

/**
 * Convert an unsigned 16-bit integer to its ASCII hexadecimal representation.
 * @warning The conversion is put in the buffer @p buffer **in reverse order**
 * and **without a final NUL character**. The caller of this function then has
 * to use the return value in order to use the characters in the buffer in the
 * right order.
 *
 * @param value The 16-bit input value to convert.
 * @param buffer A valid pointer to an allocated buffer of minimum size 4 chars.
 * @param isUpper A boolean value indicating if the conversion must be done in
 *                uppercase.
 *
 * @return The number of characters actually written to the buffer.
 */
static uint8_t
ConvertU16ToHexadecimal(uint16_t value, char * const buffer, const bool isUpper)
{
  uint8_t i;
  const uint8_t numberOfNibbles = sizeof(value) * 2;

  /* WARNING! This works only with little endian */
  for (i = 0; i < numberOfNibbles; ++i) {
    buffer[i] = GetHexDigit(value & 0xfU, isUpper);
    value >>= 4U;

    if (0 == value) {
      break;
    }
  }

  return i + 1;
}

/**
 * Convert an unsigned 16-bit integer to its ASCII octal representation.
 * @warning The conversion is put in the buffer @p buffer **in reverse order**
 * and **without a final NUL character**. The caller of this function then has
 * to use the return value in order to use the characters in the buffer in the
 * right order.
 *
 * @param value The 16-bit input value to convert.
 * @param buffer A valid pointer to an allocated buffer of minimum size 6 chars.
 *
 * @return The number of characters actually written to the buffer.
 */
static uint8_t
ConvertU16ToOctal(uint16_t value, char * const buffer)
{
  uint8_t i;
  const uint8_t numberOfGroups = 6;

  /* WARNING! This works only with little endian */
  for (i = 0; i < numberOfGroups; ++i) {
    buffer[i] = (char)((value & 07U) + '0');
    value >>= 3U;

    if (0 == value) {
      break;
    }
  }

  return i + 1;
}

/**
 * Output the padding, according to the selected options.
 *
 * @param padLength The desired pad length.
 * @param size The size of the buffer.
 * @param padChar The character to use for padding.
 * @param isNegative A boolean value indicating if the value to display is
 *                   negative.
 *
 * @return The final size of the actual output padding.
 */
static int
OutputPadding(uint8_t padLength,
              const uint8_t size,
              const char padChar,
              const bool isNegative)
{
  int total = 0;

  if (padLength > size) {
    padLength -= size;

    if (isNegative && padLength > 0) {
      --padLength;
    }

    total = padLength;

    while (padLength-- > 0) {
      putchar(padChar);
    }
  }

  return total;
}

/**
 * Output the content of the buffer.
 *
 * @param buffer A valid pointer to the buffer.
 * @param size The size of the buffer.
 */
static void
OutputBuffer(const char * const buffer, uint8_t size)
{
  while (size-- > 0) {
    putchar(buffer[size]);
  }
}

/*
 * Warning! The stack usage of this function is important! Reduce the stack
 * usage to its strict minimum.
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
      char padChar = ' ';
      bool firstDigitPassed = false;
      bool isNegative = false;
      bool rightPadded = false;
      uint8_t padLength = 0;
      uint8_t size;
      char buffer[6];

      for (++c; '\0' != *c; ++c) {
        /*
         * First, we parse formatting characters.
         * After a formatting character, we must continue to parse in the
         * current loop.
         */

        /* This test MUST be performed before the next one */
        if ('0' == *c && !firstDigitPassed) {
          padChar = '0';
          firstDigitPassed = true;

          continue;
        } else if (*c >= '0' && *c <= '9') {
          padLength = padLength * 10 + *c - '0';
          firstDigitPassed = true;

          continue;
        } else if ('-' == *c) {
          rightPadded = true;

          continue;
        } else if ('%' == *c) { /*
                                 * Then we parse "terminating characters".
                                 * After a terminating character, we must exit
                                 * the current loop.
                                 */

          ++total;
          putchar('%');

          break;
        } else if ('d' == *c || 'i' == *c || 'u' == *c) {
          unsigned int absolute;

          if ('u' == *c) {
            absolute = va_arg(args, unsigned int);
          } else {
            const int value = va_arg(args, int);
            isNegative = value < 0;
            absolute = ABS(value);
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
        } else if ('x' == *c || 'X' == *c) {
          size = ConvertU16ToHexadecimal(va_arg(args, unsigned int),
                                         buffer,
                                         'X' == *c);
        } else if ('o' == *c) {
          size = ConvertU16ToOctal(va_arg(args, unsigned int), buffer);
        } else if ('c' == *c) {
          buffer[0] = (char)va_arg(args, int);
          size = 1;
        } else {
          /*
           * We encountered an unknown character.
           * TODO: Error.
           */

          break;
        }

        /*
         * If we are here, it means we have a buffer filled with a conversion.
         * We then must print this buffer with the appropriate padding.
         */

        total += size;

        if (rightPadded) {
          /*
           * The value is to be right-padded, the operations are:
           * 1) Output the sign, if concerned.
           * 2) Output the value.
           * 3) Output the padding, overriding any pad character by a space
           *    character.
           */

          if (isNegative) {
            putchar('-');
            ++total;
          }

          OutputBuffer(buffer, size);

          total += OutputPadding(padLength, size, ' ', isNegative);
        } else {
          /*
           * The value is to be left-padded, the operations are:
           * 1) Output the sign, if concerned and only if we pad with zeros.
           * 2) Output the padding.
           * 3) Output the sign, if concerned and only if we pad with spaces.
           * 3) Output the value.
           */

          if (isNegative && '0' == padChar) {
            putchar('-');
            ++total;
          }

          total += OutputPadding(padLength, size, padChar, isNegative);

          if (isNegative && ' ' == padChar) {
            putchar('-');
            ++total;
          }

          OutputBuffer(buffer, size);
        }

        break;
      }
    } else {
      ++total;
      putchar(*c);
    }
  }

  va_end(args);

  return total;
}

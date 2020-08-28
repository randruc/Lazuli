/**
 * @date Aug 2020
 */

#ifndef PRINTF_H
#define PRINTF_H

#include <stdint.h>

/**
 * Convert an unsigned 16-bit integer to its ASCII decimal representation.
 * @warning The conversion is put in the buffer @p buffer **in reverse order**
 * and **without a final NUL character**. The caller of this function then has
 * to use the return value in order to use the characters in the buffer in the
 * right order.
 *
 * This function will always succeed.
 *
 * @param i The 16 bits input value to convert.
 * @param buffer A valid pointer to an allocated string of minimum size 5 bytes.
 *
 * @return The number of bytes actually written to the buffer.
 */
uint8_t
Printf_ConvertU16ToDecimal(uint16_t i, char buffer[]);

#endif /* PRINTF_H */

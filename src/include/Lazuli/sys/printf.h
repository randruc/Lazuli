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

/*
 * TODO: Move that to a more appropriate header file.
 */
/**
 * Produce formatted output to the serial line.
 *
 * This function is fully reentrant. No locking mechanism is provided.
 *
 * # Conversion specifiers
 *
 * The following conversion specifiers are currently allowed:
 *
 * **d**, **i** The *int* argument is converted to decimal.
 *
 * **u**        The *unsigned int* argument is converted to decimal.
 *
 * **%**        The character '%' is output.
 *
 * @param format The format string.
 * @param ... The variadic parameters.
 *
 * @return The number of characters output to the serial line, or a negative
 *         value if an error occured.
 *
 * @warning The stack usage of this function is important.
 */
int
printf(const char * format, ...);

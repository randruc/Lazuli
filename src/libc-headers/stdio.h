/**
 * @file src/include/libc-headers/stdio.h
 * @brief stdio standard header file.
 * @date Aug 2020
 * @author Remi Andruccioli
 * 
 */

#ifndef STDIO_H
#define STDIO_H

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

#endif /* STDIO_H */

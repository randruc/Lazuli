/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Serial port configuration interface.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes the interface of serial port configuration.
 * Serial port means here UART/USART device.
 *
 * The configuration is the same for input (Receive/Rx) and ouput
 * (Transmit/Tx).
 */

#ifndef LAZULI_SERIAL_H
#define LAZULI_SERIAL_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Define flags for enabling/disabling serial transmission/reception.
 *
 * These flags can be combined using bitwise OR (i.e. | operator).
 */
enum Lz_SerialEnableFlags {
  /**
   * Disable both serial transmission and reception.
   */
  LZ_SERIAL_DISABLE_ALL = 0,

  /**
   * Enable serial transmission.
   */
  LZ_SERIAL_ENABLE_TRANSMIT = POSITION(1U),

  /**
   * Enable serial reception.
   */
  LZ_SERIAL_ENABLE_RECEIVE = POSITION(2U),

  /**
   * Enable both serial transmission and reception.
   */
  LZ_SERIAL_ENABLE_ALL = LZ_SERIAL_ENABLE_RECEIVE | LZ_SERIAL_ENABLE_TRANSMIT
};

/**
 * Define the number of stop bits to use on serial line.
 */
enum Lz_SerialStopBits {
  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the first one.
   */
  __LZ_SERIAL_STOP_BITS_ENUM_BEGIN = -1,

  /** @endcond */

  /**
   * Use 1 stop bit.
   */
  LZ_SERIAL_STOP_BITS_1,

  /**
   * Use 2 stop bits.
   */
  LZ_SERIAL_STOP_BITS_2,

  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the last one.
   */
  __LZ_SERIAL_STOP_BITS_ENUM_END

  /** @endcond */
};

/**
 * Define the kind of parity bit to use on serial line.
 */
enum Lz_SerialParityBit {
  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the first one.
   */
  __LZ_SERIAL_PARITY_BIT_ENUM_BEGIN = -1,

  /** @endcond */

  /**
   * Use no parity bit.
   */
  LZ_SERIAL_PARITY_NONE,

  /**
   * Use even parity bit.
   */
  LZ_SERIAL_PARITY_EVEN,

  /**
   * Use odd parity bit.
   */
  LZ_SERIAL_PARITY_ODD,

  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the last one.
   */
  __LZ_SERIAL_PARITY_BIT_ENUM_END

  /** @endcond */
};

/**
 * Define the size of the character used on serial line.
 */
enum Lz_SerialSize {
  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the first one.
   */
  __LZ_SERIAL_SIZE_ENUM_BEGIN = -1,

  /** @endcond */

  /**
   * Use 5-bit character size on serial line.
   */
  LZ_SERIAL_SIZE_5,

  /**
   * Use 6-bit character size on serial line.
   */
  LZ_SERIAL_SIZE_6,

  /**
   * Use 7-bit character size on serial line.
   */
  LZ_SERIAL_SIZE_7,

  /**
   * Use 8-bit character size on serial line.
   */
  LZ_SERIAL_SIZE_8,

  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the last one.
   */
  __LZ_SERIAL_SIZE_ENUM_END

  /** @endcond */
};

/**
 * Define the baud rate of the serial line.
 */
enum Lz_SerialSpeed {
  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the first one.
   */
  __LZ_SERIAL_SPEED_ENUM_BEGIN = -1,

  /** @endcond */

  /**
   * Use a 2400 baud rate on the serial line.
   */
  LZ_SERIAL_SPEED_2400,

  /**
   * Use a 4800 baud rate on the serial line.
   */
  LZ_SERIAL_SPEED_4800,

  /**
   * Use a 9600 baud rate on the serial line.
   */
  LZ_SERIAL_SPEED_9600,

  /**
   * Use a 19200 baud rate on the serial line.
   */
  LZ_SERIAL_SPEED_19200,

  /**
   * @cond false
   *
   * Undocumented to user: only here for static verification.
   * This entry MUST be the last one.
   */
  __LZ_SERIAL_SPEED_ENUM_END

  /** @endcond */
};

/**
 * Represents the configuration of a serial interface.
 *
 * This configuration applies for both transmission and reception.
 */
typedef struct {
  /**
   * The enable/disable Tx/Rx options.
   */
  enum Lz_SerialEnableFlags enableFlags;

  /**
   * The number of stop bits.
   */
  enum Lz_SerialStopBits stopBits;

  /**
   * The kind of parity bit.
   */
  enum Lz_SerialParityBit parityBit;

  /**
   * The size of the character.
   */
  enum Lz_SerialSize size;

  /**
   * The baud rate.
   */
  enum Lz_SerialSpeed speed;
}Lz_SerialConfiguration;

/**
 * Retrieve the configuration of the serial line.
 *
 * This function will fill the object pointed by the parameter with current
 * serial settings.
 *
 * @param serialConfiguration A pointer to an allocated Lz_SerialConfiguration
 *                            (e.g. allocated on the stack).
 */
void
Lz_Serial_GetConfiguration(Lz_SerialConfiguration * const serialConfiguration);

/**
 * Set the configuration of the serial line.
 *
 * This function will set the different parameters of the serial line according
 * to the configuration pointed by the serialConfiguration parameter.
 *
 * @warning On the AVR platform this function will block interrupts if
 *          configuration option LZ_CONFIG_SERIAL_USE_INTERRUPTS is activated.
 *          On the AVR platform, this function will wait all transmit and
 *          receive operations has completed.
 *
 * @param serialConfiguration A pointer to an allocated Lz_SerialConfiguration
 *                            (e.g. allocated on the stack).
 */
void
Lz_Serial_SetConfiguration(Lz_SerialConfiguration * const serialConfiguration);

_EXTERN_C_DECL_END

#endif /* LAZULI_SERIAL_H */

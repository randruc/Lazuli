/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief ATmega328p USART driver.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * Describes the driver implementation for the USART of the ATMega328p.
 */

#include <stdint.h>
#include <stdio.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>
#include <Lazuli/serial.h>
#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/kernel.h>

/**
 * A constant pointer to the memory mapped Usart structure.
 */
static Usart * const usart = (Usart*)&UCSR0A;

/**
 * The current serial baud rate.
 *
 * This is used to save the current setting of the serial baud rate.
 * On AVR, the setting of the speed isn't always accurate once calculated, so
 * we use this variable to store it and be able to retrieve it easily.
 */
static enum Lz_SerialSpeed currentSerialSpeed;

int
putchar(int c)
{
  const unsigned char value = (unsigned char)c;

  while (!(usart->ucsr0a & UCSR0A_UDRE0));

  usart->udr0 = value;

  return value;
}

int
puts(const char * s)
{
  const char newLine[] = LZ_CONFIG_SERIAL_NEWLINE;
  uint8_t i;

  if (NULL == s) {
    return EOF;
  }

  while ('\0' != *s) {
    putchar(*s);
    ++s;
  }

  for (i = 0; i < sizeof(newLine) - 1; ++i) {
    putchar(newLine[i]);
  }

  return 1;
}

/**
 * Retrieve the enabling status of the serial line.
 *
 * @return An Lz_SerialEnableFlags containing the enabling status.
 */
static enum Lz_SerialEnableFlags
GetEnablingStatus(void)
{
  enum Lz_SerialEnableFlags flags = LZ_SERIAL_DISABLE_ALL;

  if (usart->ucsr0b & UCSR0B_TXEN0) {
    flags |= LZ_SERIAL_ENABLE_TRANSMIT;
  }

  if (usart->ucsr0b & UCSR0B_RXEN0) {
    flags |= LZ_SERIAL_ENABLE_RECEIVE;
  }

  return flags;
}

/**
 * Retrieve the number of stop bits currently used by the serial line.
 *
 * @return An Lz_SerialStopBits containing the number of stop bits.
 */
static enum Lz_SerialStopBits
GetStopBits(void)
{
  if (usart->ucsr0c & UCSR0C_USBS0) {
    return LZ_SERIAL_STOP_BITS_2;
  }

  return LZ_SERIAL_STOP_BITS_1;
}

/**
 * Retrieve the kind of parity currently used by the serial line.
 *
 * @return An Lz_SerialParityBit containing the kind of parity.
 */
static enum Lz_SerialParityBit
GetParityBit(void)
{
  if (usart->ucsr0c & UCSR0C_UPM01) {
    if (usart->ucsr0c & UCSR0C_UPM00) {
      return LZ_SERIAL_PARITY_ODD;
    }

    return LZ_SERIAL_PARITY_EVEN;
  }

  if (usart->ucsr0c & UCSR0C_UPM00) {
    Kernel_Panic();
  }

  return LZ_SERIAL_PARITY_NONE;
}

/**
 * Retrieve the character size currently used by the serial line.
 *
 * @return An Lz_SerialSize containing the character size.
 */
static enum Lz_SerialSize
GetSize(void)
{
  if (usart->ucsr0b & UCSR0B_UCSZ02) {
    Kernel_Panic();
  }

  if (usart->ucsr0c & UCSR0C_UCSZ01) {
    if (usart->ucsr0c & UCSR0C_UCSZ00) {
      return LZ_SERIAL_SIZE_8;
    }

    return LZ_SERIAL_SIZE_7;
  }

  if (usart->ucsr0c & UCSR0C_UCSZ00) {
    return LZ_SERIAL_SIZE_6;
  }

  return LZ_SERIAL_SIZE_5;
}

/**
 * Set the enabling/disabling of Tx/Rx of the serial line.
 *
 * @param flags An Lz_SerialEnableFlags containing enable flags.
 */
static void
SetEnablingStatus(const enum Lz_SerialEnableFlags flags)
{
  if (flags & LZ_SERIAL_ENABLE_TRANSMIT) {
    usart->ucsr0b |= UCSR0B_TXEN0;
  } else {
    usart->ucsr0b &= ~UCSR0B_TXEN0;
  }

  if (flags & LZ_SERIAL_ENABLE_RECEIVE) {
    usart->ucsr0b |= UCSR0B_RXEN0;
  } else {
    usart->ucsr0b &= ~UCSR0B_RXEN0;
  }
}

/**
 * Set USART to use 1 stop bit.
 */
static void
SetStopBits1(void)
{
  usart->ucsr0c &= ~UCSR0C_USBS0;
}

/**
 * Set USART to use 2 stop bits.
 */
static void
SetStopBits2(void)
{
  usart->ucsr0c |= UCSR0C_USBS0;
}

/**
 * Jump table to functions setting the number of stop bits of the serial line.
 *
 * @warning This table must be ordered by entry values of
 *          enum Lz_SerialStopBits.
 */
PROGMEM static
void (* const setStopBits[])(void) = {
  SetStopBits1, /**< entry: LZ_SERIAL_STOP_BITS_1 */
  SetStopBits2  /**< entry: LZ_SERIAL_STOP_BITS_2 */
};

/** @cond false */
STATIC_ASSERT
(
 ELEMENTS_COUNT(setStopBits) == __LZ_SERIAL_STOP_BITS_ENUM_END,
 all_entries_of_enum_Lz_SerialStopBits_must_be_set_in_setStopBits
);
/** @endcond */

/**
 * Set the number of stop bits of the serial line.
 *
 * @param stopBits An Lz_SerialStopBits defining the number of stop bits.
 */
static void
SetStopBits(const enum Lz_SerialStopBits stopBits)
{
  void (*jump)(void);

  if (LZ_CONFIG_CHECK_WRONG_ENUM_ENTRIES_IN_SERIAL) {
    if (stopBits <= __LZ_SERIAL_STOP_BITS_ENUM_BEGIN ||
        stopBits >= __LZ_SERIAL_STOP_BITS_ENUM_END) {
      Kernel_Panic();
    }
  }

  jump = Arch_LoadFunctionPointerFromProgmem(&setStopBits[stopBits]);
  jump();
}

/**
 * Set no parity bit.
 */
static void
SetParityBitNone(void)
{
  usart->ucsr0c &= ~(UCSR0C_UPM01 | UCSR0C_UPM00);
}

/**
 * Set even parity bit.
 */
static void
SetParityBitEven(void)
{
  usart->ucsr0c |= UCSR0C_UPM01;
  usart->ucsr0c &= ~UCSR0C_UPM00;
}

/**
 * Set odd parity bit.
 */
static void
SetParityBitOdd(void)
{
  usart->ucsr0c |= (UCSR0C_UPM01 | UCSR0C_UPM00);
}

/**
 * Jump table to functions setting the kind of parity of the serial line.
 *
 * @warning This table must be ordered by entry values of
 *          enum Lz_SerialParityBit.
 */
PROGMEM static
void (* const setParityBit[])(void) = {
  SetParityBitNone, /**< entry: LZ_SERIAL_PARITY_NONE */
  SetParityBitEven, /**< entry: LZ_SERIAL_PARITY_EVEN */
  SetParityBitOdd   /**< entry: LZ_SERIAL_PARITY_ODD  */
};

/** @cond false */
STATIC_ASSERT
(
 ELEMENTS_COUNT(setParityBit) == __LZ_SERIAL_PARITY_BIT_ENUM_END,
 all_entries_of_enum_Lz_SerialParityBit_must_be_set_in_setParityBit
);
/** @endcond */

/**
 * Set the kind of parity bit of the serial line.
 *
 * @param parityBit An Lz_SerialParityBit defining the kind of parity.
 */
static void
SetParityBit(const enum Lz_SerialParityBit parityBit)
{
  void (*jump)(void);

  if (LZ_CONFIG_CHECK_WRONG_ENUM_ENTRIES_IN_SERIAL) {
    if (parityBit <= __LZ_SERIAL_PARITY_BIT_ENUM_BEGIN ||
        parityBit >= __LZ_SERIAL_PARITY_BIT_ENUM_END) {
      Kernel_Panic();
    }
  }

  jump = Arch_LoadFunctionPointerFromProgmem(&setParityBit[parityBit]);
  jump();
}

/**
 * Set character size to 5 bits.
 */
static void
SetSize5(void)
{
  usart->ucsr0b &= ~UCSR0B_UCSZ02;
  usart->ucsr0c &= ~(UCSR0C_UCSZ01 | UCSR0C_UCSZ00);
}

/**
 * Set character size to 6 bits.
 */
static void
SetSize6(void)
{
  usart->ucsr0b &= ~UCSR0B_UCSZ02;
  usart->ucsr0c &= ~UCSR0C_UCSZ01;
  usart->ucsr0c |= UCSR0C_UCSZ00;
}

/**
 * Set character size to 7 bits.
 */
static void
SetSize7(void)
{
  usart->ucsr0b &= ~UCSR0B_UCSZ02;
  usart->ucsr0c |= UCSR0C_UCSZ01;
  usart->ucsr0c &= ~UCSR0C_UCSZ00;
}

/**
 * Set character size to 8 bits.
 */
static void
SetSize8(void)
{
  usart->ucsr0b &= ~UCSR0B_UCSZ02;
  usart->ucsr0c |= (UCSR0C_UCSZ01 | UCSR0C_UCSZ00);
}

/**
 * Jump table to functions setting character sizes of the serial line.
 *
 * @warning This table must be ordered by entry values of enum Lz_SerialSize.
 */
PROGMEM static
void (* const setSize[])(void) = {
  SetSize5, /**< entry: LZ_SERIAL_SIZE_5 */
  SetSize6, /**< entry: LZ_SERIAL_SIZE_6 */
  SetSize7, /**< entry: LZ_SERIAL_SIZE_7 */
  SetSize8  /**< entry: LZ_SERIAL_SIZE_8 */
};

/** @cond false */
STATIC_ASSERT
(
 ELEMENTS_COUNT(setSize) == __LZ_SERIAL_SIZE_ENUM_END,
 all_entries_of_enum_Lz_SerialSize_must_be_set_in_setSize
);
/** @endcond */

/**
 * Set the character size of the serial line.
 *
 * @param size An Lz_SerialSize defining the character size.
 */
static void
SetSize(const enum Lz_SerialSize size)
{
  void (* jump)(void);

  if (LZ_CONFIG_CHECK_WRONG_ENUM_ENTRIES_IN_SERIAL) {
    if (size <= __LZ_SERIAL_SIZE_ENUM_BEGIN ||
        size >= __LZ_SERIAL_SIZE_ENUM_END) {
      Kernel_Panic();
    }
  }

  jump = Arch_LoadFunctionPointerFromProgmem(&setSize[size]);
  jump();
}

/**
 * Jump table containing the setting values of UBRR0L and UBRR0H registers for
 * baud rates defined in Lz_SerialSpeed.
 *
 * @warning This table must be ordered by entry values of enum Lz_SerialSpeed.
 */
PROGMEM static const
uint16_t serialSpeedRegisterValue[] = {
  (uint16_t)416, /**< entry: LZ_SERIAL_SPEED_2400  */
  (uint16_t)207, /**< entry: LZ_SERIAL_SPEED_4800  */
  (uint16_t)103, /**< entry: LZ_SERIAL_SPEED_9600  */
  (uint16_t)51   /**< entry: LZ_SERIAL_SPEED_19200 */
};

/** @cond false */
STATIC_ASSERT
(
 ELEMENTS_COUNT(serialSpeedRegisterValue) == __LZ_SERIAL_SPEED_ENUM_END,
 all_entries_of_enum_Lz_SerialSpeed_must_be_set_in_serialSpeedRegisterValue
);
/** @endcond */

/**
 * Set the baud rate of the serial line.
 *
 * @param speed An Lz_SerialSpeed value defining the baud rate.
 */
static void
SetSpeed(const enum Lz_SerialSpeed speed)
{
  uint16_t registerValue;

  if (LZ_CONFIG_CHECK_WRONG_ENUM_ENTRIES_IN_SERIAL) {
    if (speed <= __LZ_SERIAL_SPEED_ENUM_BEGIN ||
        speed >= __LZ_SERIAL_SPEED_ENUM_END) {
      Kernel_Panic();
    }
  }

  registerValue = Arch_LoadU16FromProgmem(&serialSpeedRegisterValue[speed]);

  usart->ubrr0l = LO8(registerValue);
  usart->ubrr0h = HI8(registerValue);

  currentSerialSpeed = speed;
}

void
Arch_GetSerialConfiguration(Lz_SerialConfiguration * const configuration)
{
  configuration->enableFlags = GetEnablingStatus();
  configuration->stopBits = GetStopBits();
  configuration->parityBit = GetParityBit();
  configuration->size = GetSize();
  configuration->speed = currentSerialSpeed;
}

void
Arch_SetSerialConfiguration(const Lz_SerialConfiguration * const configuration)
{
  InterruptsStatus interruptsStatus;

  /* Wait all receive and transmit operations has completed */
  /* TODO: This one doesn't work as expected */
  /* while (!(usart->ucsr0a & UCSR0A_TXC0) || (usart->ucsr0a & UCSR0A_RXC0)); */

  /*
   * From the ATmega328p datasheet:
   * "For interrupt driven USART operation, the Global Interrupt Flag should be
   * cleared (and interrupts globally disabled) when doing the initialization."
   */
  if (LZ_CONFIG_SERIAL_USE_INTERRUPTS) {
    interruptsStatus = Arch_DisableInterruptsGetStatus();
  }

  SetEnablingStatus(configuration->enableFlags);
  SetStopBits(configuration->stopBits);
  SetParityBit(configuration->parityBit);
  SetSize(configuration->size);
  SetSpeed(configuration->speed);

  if (LZ_CONFIG_SERIAL_USE_INTERRUPTS) {
    Arch_RestoreInterruptsStatus(interruptsStatus);
  }
}

void
Arch_InitSerial(void)
{
  const Lz_SerialConfiguration serialConfiguration = {
    LZ_SERIAL_DISABLE_ALL, /**< member: enableFlags */
    LZ_SERIAL_STOP_BITS_1, /**< member: stopBits    */
    LZ_SERIAL_PARITY_NONE, /**< member: parityBits  */
    LZ_SERIAL_SIZE_8,      /**< member: size        */
    LZ_SERIAL_SPEED_4800   /**< member: speed       */
  };

  Arch_SetSerialConfiguration(&serialConfiguration);

  /* Set USART to asynchronous */
  CLEAR_BITS(usart->ucsr0c, uint8_t, UCSR0C_UMSEL01 | UCSR0C_UMSEL00);
}

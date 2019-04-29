/**
 * @file ken/unit-tests/unit_tests.c
 * @brief Lazuli kernel unit tests.
 * @date Apr 2019
 * @author Remi Andruccioli
 *
 * This file contains unit tests to test the kernel, along with a
 * micro-framework to perform unit tests on the target machine.
 */

#include <Lazuli/common.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/kernel.h>

#define ASSERT(C) Assert(C, __LINE__)
#define UNIT_TEST(N) static void N(void)

static void
Assert(const bool cond, const uint16_t line);

UNIT_TEST(GlobalEnableDisableInterrupts)
{
  Arch_EnableInterrupts();
  ASSERT(Arch_AreInterruptsEnabled());

  Arch_DisableInterrupts();
  ASSERT(!Arch_AreInterruptsEnabled());
}

UNIT_TEST(GlobalEnableDisableInterruptsWithStatus_1)
{
  InterruptsStatus interruptsStatus;

  Arch_DisableInterrupts();

  interruptsStatus = Arch_DisableInterruptsGetStatus();
  ASSERT(!Arch_AreInterruptsEnabled());

  Arch_RestoreInterruptsStatus(interruptsStatus);
  ASSERT(!Arch_AreInterruptsEnabled());
}

UNIT_TEST(GlobalEnableDisableInterruptsWithStatus_2)
{
  InterruptsStatus interruptsStatus;

  Arch_EnableInterrupts();

  interruptsStatus = Arch_DisableInterruptsGetStatus();
  ASSERT(!Arch_AreInterruptsEnabled());

  Arch_RestoreInterruptsStatus(interruptsStatus);
  ASSERT(Arch_AreInterruptsEnabled());

  Arch_DisableInterrupts();
}

/*
 * TODO: This test is machine specific! It assumes that the length of a pointer
 * is 16 bits!
 */
UNIT_TEST(ReverseBytesOfFunctionPointer_1)
{
  void (* const oldPointer)(void) = (void (*)(void))0xabcd;
  void (* const newPointer)(void) = ReverseBytesOfFunctionPointer(oldPointer);
  void (* const expectedPointer)(void) = (void (*)(void))0xcdab;

  ASSERT(expectedPointer == newPointer);
}

/*
 * TODO: This test is machine specific! It assumes that the length of a pointer
 * is 16 bits!
 */
UNIT_TEST(ReverseBytesOfFunctionPointer_2)
{
  void (* const oldPointer)(void) = (void (*)(void))0xff;
  void (* const newPointer)(void) = ReverseBytesOfFunctionPointer(oldPointer);
  void (* const expectedPointer)(void) = (void (*)(void))0xff00;

  ASSERT(expectedPointer == newPointer);
}

__progmem static void (* const tests[])(void) = {
  GlobalEnableDisableInterrupts,
  GlobalEnableDisableInterruptsWithStatus_1,
  GlobalEnableDisableInterruptsWithStatus_2,
  ReverseBytesOfFunctionPointer_1,
  ReverseBytesOfFunctionPointer_2
};

static void
Assert(const bool cond, const uint16_t line)
{
  if (!cond) {
    Usart_PrintRawString("KO: ");
    Usart_HexaPrint_u16(line);
    Usart_NewLine();
  }
}

static void
EnableSerialTransmission() {
  Lz_SerialConfiguration serialConfiguration;

  Lz_Serial_GetConfiguration(&serialConfiguration);
  serialConfiguration.enableFlags = LZ_SERIAL_ENABLE_TRANSMIT;
  Lz_Serial_SetConfiguration(&serialConfiguration);
}

void
main(void)
{
  size_t i;
  void (*test)(void);

  EnableSerialTransmission();

  Usart_PutChar('-');
  Usart_NewLine();

  for (i = 0; i < ELEMENTS_COUNT(tests); ++i) {
    test = Arch_LoadFunctionPointerFromProgmem(&tests[i]);
    test();
  }

  Usart_PutChar('.');
  Usart_NewLine();

  for (;;);
}

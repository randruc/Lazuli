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

#define ASSERT(C) Assert(C, __LINE__)
#define UNIT_TEST(N) void N(void)

void
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

__progmem static void (* const tests[])(void) = {
  GlobalEnableDisableInterrupts,
  GlobalEnableDisableInterruptsWithStatus_1,
  GlobalEnableDisableInterruptsWithStatus_2
};

void
Assert(const bool cond, const uint16_t line)
{
  if (!cond) {
    Usart_PrintRawString("KO: ");
    Usart_HexaPrint_u16(line);
    Usart_NewLine();
  }
}

void
main(void)
{
  size_t i;
  void (*test)(void);

  Usart_Init();

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

/**
 * @file kern/unit-tests/unit_tests.c
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
#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/scheduler_base.h>

/**
 * Perform an assertion inside a unit test.
 *
 * If this assertion is false the corresponding line number will be printed on
 * serial line.
 *
 * @param C The assertion to perform.
 */
#define ASSERT(C) Assert(C, __LINE__)

/**
 * Declare a unit test.
 *
 * @param N The name of the test to declare.
 */
#define UNIT_TEST(N) static void N(void)

/**
 * Internal function used to perform an assertion.
 *
 * @param cond The condition to test.
 * @param line The line number of the assertion.
 */
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

/*
 * TODO: This test is machine specific! It assumes that the length of a pointer
 * is 16 bits!
 */
UNIT_TEST(ReverseBytesOfFunctionPointer_3)
{
  void (* const oldPointer)(void) = (void (*)(void))0x2121;
  void (* const newPointer)(void) = ReverseBytesOfFunctionPointer(oldPointer);

  ASSERT(oldPointer == newPointer);
}

PROGMEM static const uint8_t u8Value = 4;
UNIT_TEST(LoadU8FromProgmem_1)
{
  uint8_t target = 0;

  target = Arch_LoadU8FromProgmem(&u8Value);

  ASSERT(4 == target);
}

PROGMEM static const uint8_t u8Values[] = { 4, 3, 2 };
UNIT_TEST(LoadU8FromProgmem_2)
{
  uint8_t target = 0;

  target = Arch_LoadU8FromProgmem(&u8Values[2]);

  ASSERT(2 == target);
}

PROGMEM static const void *pointer = (void*)0x4562;
UNIT_TEST(LoadPointerFromProgmem_1)
{
  void *target = NULL;

  target = Arch_LoadPointerFromProgmem(&pointer);

  ASSERT((void*)0x4562 == target);
}

PROGMEM static const uint8_t * const pointers[] =
  {
    (uint8_t*)0x4562,
    (uint8_t*)0x1278,
    (uint8_t*)0x6325
  };
UNIT_TEST(LoadPointerFromProgmem_2)
{
  uint8_t *target = NULL;

  target = Arch_LoadPointerFromProgmem(&pointers[1]);

  ASSERT((uint8_t*)0x1278 == target);
}

PROGMEM static void (* const functionPointer)(void) = LoadU8FromProgmem_1;
UNIT_TEST(LoadFunctionPointerFromProgmem_1)
{
  void (*pointer)(void) = NULL;

  pointer = Arch_LoadFunctionPointerFromProgmem(&functionPointer);

  ASSERT(LoadU8FromProgmem_1 == pointer);
}

PROGMEM static void (* const functionPointers[])(void) = {
  LoadPointerFromProgmem_1,
  LoadPointerFromProgmem_2,
  LoadU8FromProgmem_1
};
UNIT_TEST(LoadFunctionPointerFromProgmem_2)
{
  void (*pointer)(void) = NULL;

  pointer = Arch_LoadFunctionPointerFromProgmem(&functionPointers[1]);

  ASSERT(LoadPointerFromProgmem_2 == pointer);
}

PROGMEM static const uint16_t u16Value = 10658;
UNIT_TEST(LoadU16FromProgmem_1)
{
  uint16_t target = 0;

  target = Arch_LoadU16FromProgmem(&u16Value);

  ASSERT(10658 == target);
}

PROGMEM static const uint16_t u16Values[] = { 40236, 30458, 20785 };
UNIT_TEST(LoadU16FromProgmem_2)
{
  uint16_t target = 0;

  target = Arch_LoadU16FromProgmem(&u16Values[2]);

  ASSERT(20785 == target);
}

typedef struct {
  uint16_t u16Value;
  void (*functionPointer)(void);
  bool b;
}TestStruct;

PROGMEM static const TestStruct testStruct = {
  32458,
  LoadU16FromProgmem_2,
  true
};
UNIT_TEST(LoadFromProgmem_1)
{
  TestStruct target = {
    30124,
    LoadFunctionPointerFromProgmem_2,
    false
  };

  Arch_LoadFromProgmem(&testStruct, &target, sizeof(TestStruct));

  ASSERT(32458 == target.u16Value);
  ASSERT(LoadU16FromProgmem_2 == target.functionPointer);
  ASSERT(true == target.b);
}

PROGMEM static const TestStruct testStructs[] = {
  {
    32458,
    LoadU16FromProgmem_2,
    true
  },
  {
    51201,
    LoadFromProgmem_1,
    true
  },
  {
    5,
    LoadU8FromProgmem_2,
    false
  },
};
UNIT_TEST(LoadFromProgmem_2)
{
  TestStruct target = {
    30124,
    LoadFunctionPointerFromProgmem_2,
    false
  };

  Arch_LoadFromProgmem(&testStructs[2], &target, sizeof(TestStruct));

  ASSERT(5 == target.u16Value);
  ASSERT(LoadU8FromProgmem_2 == target.functionPointer);
  ASSERT(false == target.b);
}

/**
 * Array of function pointers referencing all unit tests to execute.
 */
PROGMEM static void (* const tests[])(void) = {
  GlobalEnableDisableInterrupts,
  GlobalEnableDisableInterruptsWithStatus_1,
  GlobalEnableDisableInterruptsWithStatus_2,
  ReverseBytesOfFunctionPointer_1,
  ReverseBytesOfFunctionPointer_2,
  ReverseBytesOfFunctionPointer_3,
  LoadU8FromProgmem_1,
  LoadU8FromProgmem_2,
  LoadPointerFromProgmem_1,
  LoadPointerFromProgmem_2,
  LoadFunctionPointerFromProgmem_1,
  LoadFunctionPointerFromProgmem_2,
  LoadU16FromProgmem_1,
  LoadU16FromProgmem_2,
  LoadFromProgmem_1,
  LoadFromProgmem_2
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

/**
 * Activate serial transmission.
 */
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

  Usart_NewLine();
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
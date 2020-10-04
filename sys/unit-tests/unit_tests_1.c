/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Lazuli kernel unit tests suite part 1 - Printf.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file contains unit tests for printf-related functionnalities.
 *
 * This file is part 1, so it must be executed first when all unit tests sets
 * are to be executed. This is because it tests printf-related functionnalities,
 * so the next unit tests sets can safely rely on printing if needed.
 */

#include "unit_tests_common.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>

#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/printf.h>

DEPENDENCY_ON_MODULE(PRINTF);
DEPENDENCY_ON_MODULE(SERIAL);

void
Variadic_1(uint16_t i, ...)
{
  va_list args;
  uint8_t u8 = 0;
  int8_t s8 = 0;
  uint16_t u16 = 0;
  int16_t s16 = 0;
  uint32_t u32 = 0;
  int32_t s32 = 0;
  char c = '\0';
  bool b = false;
  void *p = NULL;

  ASSERT(42 == i);

  va_start(args, i);

  u8 = va_arg(args, int);
  ASSERT(56 == u8);

  s8 = va_arg(args, int);
  ASSERT(-2 == s8);

  u16 = va_arg(args, uint16_t);
  ASSERT(23796 == u16);

  s16 = va_arg(args, int16_t);
  ASSERT(-784 == s16);

  u32 = va_arg(args, uint32_t);
  ASSERT(1569125701 == u32);

  s32 = va_arg(args, int32_t);
  ASSERT(-2000364785 == s32);

  c = va_arg(args, int);
  ASSERT('y' == c);

  b = va_arg(args, int);
  ASSERT(b);

  p = va_arg(args, void*);
  ASSERT((void*)12458 == p);

  va_end(args);
}

UNIT_TEST(Variadic_Function_1)
{
  uint8_t u8 = 56;
  int8_t s8 = -2;
  uint16_t u16 = 23796;
  int16_t s16 = -784;
  uint32_t u32 = 1569125701;
  int32_t s32 = -2000364785;
  char c = 'y';
  bool b = true;
  void *p = (void*)12458;

  Variadic_1((uint16_t)42,
             u8,
             s8,
             u16,
             s16,
             u32,
             s32,
             c,
             b,
             p);

}

void
Variadic_2(char *p, ...)
{
  va_list args;
  bool b = false;
  int32_t i = 0;
  char *str = NULL;

  ASSERT(NULL == p);

  va_start(args, p);

  b = va_arg(args, int);
  ASSERT(b);

  i = va_arg(args, int32_t);
  ASSERT((int32_t)-1 == i);

  str = va_arg(args, char*);
  ASSERT(StringsAreEqual("Test", str));

  va_end(args);
}

UNIT_TEST(Variadic_Function_2)
{
  bool b = true;
  int32_t i = -1;
  char *str = "Test";

  Variadic_2(NULL,
             b,
             i,
             str);
}

void
Variadic_3(uint16_t u, ...)
{
  va_list args;
  uint16_t i;
  uint8_t j = 0;

  ASSERT(15 == u);

  va_start(args, u);

  for (;;) {
    i = va_arg(args, uint16_t);
    if (0 == i) {
      break;
    }

    ASSERT((uint16_t)925 + j == i);

    ++j;
  }

  ASSERT(8 == j);

  va_end(args);
}

UNIT_TEST(Variadic_Function_3)
{
  uint16_t i = 15;
  uint16_t j = 925;

  Variadic_3(i,
             j,
             j + 1,
             j + 2,
             j + 3,
             j + 4,
             j + 5,
             j + 6,
             j + 7,
             (uint16_t)0);
}

struct Size3 {
  int8_t v[3];
};

void
Variadic_4(char *p, ...)
{
  va_list args;
  bool b = false;
  int32_t i = 0;
  struct Size3 s = { 0 };
  char *str = NULL;

  ASSERT(NULL == p);

  va_start(args, p);

  b = va_arg(args, int);
  ASSERT(b);

  i = va_arg(args, int32_t);
  ASSERT((int32_t)-1 == i);

  s = va_arg(args, struct Size3);
  ASSERT(s.v[0] == 127);
  ASSERT(s.v[1] == -12);
  ASSERT(s.v[2] == 9);

  str = va_arg(args, char*);
  ASSERT(StringsAreEqual("Test", str));

  va_end(args);
}

UNIT_TEST(Variadic_Function_4)
{
  bool b = true;
  int32_t i = -1;
  struct Size3 s = { { 127, -12, 9 } };
  char *str = "Test";

  Variadic_4(NULL,
             b,
             i,
             s,
             str);
}

void
Variadic_5(char *p, va_list args)
{
  bool b = false;
  int32_t i = 0;
  struct Size3 s = { 0 };
  char *str = NULL;

  ASSERT(NULL == p);

  b = va_arg(args, int);
  ASSERT(b);

  i = va_arg(args, int32_t);
  ASSERT((int32_t)-1 == i);

  s = va_arg(args, struct Size3);
  ASSERT(s.v[0] == 127);
  ASSERT(s.v[1] == -12);
  ASSERT(s.v[2] == 9);

  str = va_arg(args, char*);
  ASSERT(StringsAreEqual("Test", str));

  va_end(args);
}

void
Variadic_Passthrough_5(char *p, ...) {
  va_list args;

  va_start(args, p);

  Variadic_5(p, args);

  va_end(args);
}

UNIT_TEST(Variadic_Function_Passtrough_5)
{
  bool b = false;
  int32_t i = 0;
  struct Size3 s = { 0 };
  char *str = NULL;

  b = true;
  i = -1;
  s.v[0] = 127;
  s.v[1] = -12;
  s.v[2] = 9;
  str = "Test";

  Variadic_Passthrough_5(NULL,
                         b,
                         i,
                         s,
                         str);
}

UNIT_TEST(ConvertU16ToDecimal_1)
{
  const uint16_t i = 456;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(3 == size);
  ASSERT('6' == buffer[0]);
  ASSERT('5' == buffer[1]);
  ASSERT('4' == buffer[2]);
}

UNIT_TEST(ConvertU16ToDecimal_2)
{
  const uint16_t i = 0;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(1 == size);
  ASSERT('0' == buffer[0]);
}

UNIT_TEST(ConvertU16ToDecimal_3)
{
  const uint16_t i = 65535;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(5 == size);
  ASSERT('5' == buffer[0]);
  ASSERT('3' == buffer[1]);
  ASSERT('5' == buffer[2]);
  ASSERT('5' == buffer[3]);
  ASSERT('6' == buffer[4]);
}

UNIT_TEST(ConvertU16ToDecimal_4)
{
  const uint16_t i = 12;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(2 == size);
  ASSERT('2' == buffer[0]);
  ASSERT('1' == buffer[1]);
}

UNIT_TEST(ConvertU16ToDecimal_5)
{
  const uint16_t i = 7;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(1 == size);
  ASSERT('7' == buffer[0]);
}

UNIT_TEST(ConvertU16ToDecimal_6)
{
  const uint16_t i = 10000;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(5 == size);
  ASSERT('0' == buffer[0]);
  ASSERT('0' == buffer[1]);
  ASSERT('0' == buffer[2]);
  ASSERT('0' == buffer[3]);
  ASSERT('1' == buffer[4]);
}

UNIT_TEST(ConvertU16ToDecimal_7)
{
  const uint16_t i = 22222;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(5 == size);
  ASSERT('2' == buffer[0]);
  ASSERT('2' == buffer[1]);
  ASSERT('2' == buffer[2]);
  ASSERT('2' == buffer[3]);
  ASSERT('2' == buffer[4]);
}

UNIT_TEST(ConvertU16ToDecimal_8)
{
  const uint16_t i = 3141;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(4 == size);
  ASSERT('1' == buffer[0]);
  ASSERT('4' == buffer[1]);
  ASSERT('1' == buffer[2]);
  ASSERT('3' == buffer[3]);
}

UNIT_TEST(ConvertU16ToDecimal_9)
{
  const uint16_t i = 1;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(1 == size);
  ASSERT('1' == buffer[0]);
}

UNIT_TEST(ConvertU16ToDecimal_10)
{
  const uint16_t i = 10;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(2 == size);
  ASSERT('0' == buffer[0]);
  ASSERT('1' == buffer[1]);
}

UNIT_TEST(ConvertU16ToDecimal_11)
{
  const uint16_t i = 2212;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(4 == size);
  ASSERT('2' == buffer[0]);
  ASSERT('1' == buffer[1]);
  ASSERT('2' == buffer[2]);
  ASSERT('2' == buffer[3]);
}

UNIT_TEST(ConvertU16ToDecimal_12)
{
  const uint16_t i = 1091;
  char buffer[5];
  const uint8_t size = Printf_ConvertU16ToDecimal(i, buffer);

  ASSERT(4 == size);
  ASSERT('1' == buffer[0]);
  ASSERT('9' == buffer[1]);
  ASSERT('0' == buffer[2]);
  ASSERT('1' == buffer[3]);
}

UNIT_TEST(Printf_1)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("2019:%u" LZ_CONFIG_SERIAL_NEWLINE , 2019);

  ASSERT(11 == total);
}

UNIT_TEST(Printf_2)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("-1205:%d" LZ_CONFIG_SERIAL_NEWLINE, -1205);

  ASSERT(13 == total);
}

UNIT_TEST(Printf_3)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("0:%d" LZ_CONFIG_SERIAL_NEWLINE, 0);

  ASSERT(5 == total);
}

UNIT_TEST(Printf_4)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("1:%d" LZ_CONFIG_SERIAL_NEWLINE, 1);

  ASSERT(5 == total);
}

UNIT_TEST(Printf_5)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("65535:%u" LZ_CONFIG_SERIAL_NEWLINE, 65535u);

  ASSERT(13 == total);
}

UNIT_TEST(Printf_6)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("-1:%d" LZ_CONFIG_SERIAL_NEWLINE, -1);

  ASSERT(7 == total);
}

UNIT_TEST(Printf_7)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("48620:%u" LZ_CONFIG_SERIAL_NEWLINE, 48620u);

  ASSERT(13 == total);
}

UNIT_TEST(Printf_8)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("0000048620:%010u" LZ_CONFIG_SERIAL_NEWLINE, 48620u);

  ASSERT(23 == total);
}

UNIT_TEST(Printf_9)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("25631:%01u" LZ_CONFIG_SERIAL_NEWLINE, 25631u);

  ASSERT(13 == total);
}

UNIT_TEST(Printf_10)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("228:%03u" LZ_CONFIG_SERIAL_NEWLINE, 228u);

  ASSERT(9 == total);
}

UNIT_TEST(Printf_11)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("-1715:%03d" LZ_CONFIG_SERIAL_NEWLINE, -1715);

  ASSERT(13 == total);
}

UNIT_TEST(Printf_12)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("-004:%04d" LZ_CONFIG_SERIAL_NEWLINE, -4);

  ASSERT(11 == total);
}

UNIT_TEST(Printf_13)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("-563:%02d" LZ_CONFIG_SERIAL_NEWLINE, -563);

  ASSERT(11 == total);
}

UNIT_TEST(Printf_14)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("00001963:%08d" LZ_CONFIG_SERIAL_NEWLINE, 1963);

  ASSERT(19 == total);
}

UNIT_TEST(Printf_15)
{
  int total;

  Usart_PrintRawString("C:");

  total = printf("065535:%06u" LZ_CONFIG_SERIAL_NEWLINE, 65535u);

  ASSERT(15 == total);
}

UNIT_TEST(Printf_16)
{
  int total;

  Usart_PrintRawString("C:0:");

  total = printf("%x", 0);

  Usart_NewLine();

  ASSERT(1 == total);
}

UNIT_TEST(Printf_17)
{
  int total;

  Usart_PrintRawString("C:ff:");

  total = printf("%x", 255);

  Usart_NewLine();

  ASSERT(2 == total);
}

UNIT_TEST(Printf_18)
{
  int total;

  Usart_PrintRawString("C:FF:");

  total = printf("%X", 255);

  Usart_NewLine();

  ASSERT(2 == total);
}

UNIT_TEST(Printf_19)
{
  int total;

  Usart_PrintRawString("C:FA02:");

  total = printf("%X", 0xfa02u);

  Usart_NewLine();

  ASSERT(4 == total);
}

UNIT_TEST(Printf_20)
{
  int total;

  Usart_PrintRawString("C:abcd:");

  total = printf("%x", 0xabcdu);

  Usart_NewLine();

  ASSERT(4 == total);
}

UNIT_TEST(Printf_21)
{
  int total;

  Usart_PrintRawString("C:ffff:");

  total = printf("%x", 0xffffu);

  Usart_NewLine();

  ASSERT(4 == total);
}

UNIT_TEST(Printf_22)
{
  int total;

  Usart_PrintRawString("C:FFFF:");

  total = printf("%X", 0xffffu);

  Usart_NewLine();

  ASSERT(4 == total);
}

UNIT_TEST(Printf_23)
{
  int total;

  Usart_PrintRawString("C:FE2534:");

  total = printf("%X%d", 0xfe25, 34);

  Usart_NewLine();

  ASSERT(6 == total);
}

UNIT_TEST(Printf_24)
{
  int total;

  Usart_PrintRawString("C:00c:");

  total = printf("%03x", 0xc);

  Usart_NewLine();

  ASSERT(3 == total);
}

UNIT_TEST(Printf_25)
{
  int total;

  Usart_PrintRawString("C:000fe:");

  total = printf("%05x", 0xfe);

  Usart_NewLine();

  ASSERT(5 == total);
}

UNIT_TEST(Printf_26)
{
  int total;

  Usart_PrintRawString("C:DE:");

  total = printf("%01X", 0xde);

  Usart_NewLine();

  ASSERT(2 == total);
}

UNIT_TEST(Printf_27)
{
  int total;

  Usart_PrintRawString("C:deadbeef:");

  total = printf("%02x%04x", 0xdead, 0xbeef);

  Usart_NewLine();

  ASSERT(8 == total);
}

UNIT_TEST(Printf_28)
{
  int total;

  Usart_PrintRawString("C:The 1st of march.:");

  total = printf("The %xst of march.", 1);

  Usart_NewLine();

  ASSERT(17 == total);
}

UNIT_TEST(Printf_29)
{
  int total;

  Usart_PrintRawString("C:The 1st of march.:");

  total = printf("The %Xst of march.", 1);

  Usart_NewLine();

  ASSERT(17 == total);
}

UNIT_TEST(Printf_30)
{
  int total;

  Usart_PrintRawString("C:The 1st of march.:");

  total = printf("The %dst of march.", 1);

  Usart_NewLine();

  ASSERT(17 == total);
}

UNIT_TEST(Printf_31)
{
  int total;

  Usart_PrintRawString("C:0:");

  total = printf("%o", 0);

  Usart_NewLine();

  ASSERT(1 == total);
}

UNIT_TEST(Printf_32)
{
  int total;

  Usart_PrintRawString("C:1:");

  total = printf("%o", 1);

  Usart_NewLine();

  ASSERT(1 == total);
}

UNIT_TEST(Printf_33)
{
  int total;

  Usart_PrintRawString("C:177777:");

  total = printf("%o", 0xffff);

  Usart_NewLine();

  ASSERT(6 == total);
}

UNIT_TEST(Printf_34)
{
  int total;

  Usart_PrintRawString("C:54124:");

  total = printf("%o", 054124);

  Usart_NewLine();

  ASSERT(5 == total);
}

UNIT_TEST(Printf_35)
{
  int total;

  Usart_PrintRawString("C:15:");

  total = printf("%o", 015);

  Usart_NewLine();

  ASSERT(2 == total);
}

UNIT_TEST(Printf_36)
{
  int total;

  Usart_PrintRawString("C:777:");

  total = printf("%o", 0777);

  Usart_NewLine();

  ASSERT(3 == total);
}

UNIT_TEST(Printf_37)
{
  int total;

  Usart_PrintRawString("C:1000:");

  total = printf("%o", 01000);

  Usart_NewLine();

  ASSERT(4 == total);
}

UNIT_TEST(Printf_38)
{
  int total;

  Usart_PrintRawString("C:A:");

  total = printf("%c", 'A');

  Usart_NewLine();

  ASSERT(1 == total);
}

UNIT_TEST(Printf_39)
{
  int total;

  Usart_PrintRawString("C:%:");

  total = printf("%c", '%');

  Usart_NewLine();

  ASSERT(1 == total);
}

UNIT_TEST(Printf_40)
{
  int total;

  Usart_PrintRawString("C: :");

  total = printf("%c", ' ');

  Usart_NewLine();

  ASSERT(1 == total);
}

UNIT_TEST(Printf_41)
{
  int total;

  Usart_PrintRawString("C:0:");

  total = printf("%c", '0');

  Usart_NewLine();

  ASSERT(1 == total);
}

UNIT_TEST(Printf_42)
{
  int total;

  Usart_PrintRawString("C:12!f9:");

  total = printf("%o%c%x%d", 012, '!', 0xf, 9);

  Usart_NewLine();

  ASSERT(5 == total);
}

UNIT_TEST(Printf_43)
{
  int total;

  Usart_PrintRawString("C:AZE,12:");

  total = printf("%XZE%c%o", 0xA, ',', 012);

  Usart_NewLine();

  ASSERT(6 == total);
}

void
ExecuteTests(void)
{
  Variadic_Function_1();
  Variadic_Function_2();
  Variadic_Function_3();
  Variadic_Function_4();
  Variadic_Function_Passtrough_5();
  ConvertU16ToDecimal_1();
  ConvertU16ToDecimal_2();
  ConvertU16ToDecimal_3();
  ConvertU16ToDecimal_4();
  ConvertU16ToDecimal_5();
  ConvertU16ToDecimal_6();
  ConvertU16ToDecimal_7();
  ConvertU16ToDecimal_8();
  ConvertU16ToDecimal_9();
  ConvertU16ToDecimal_10();
  ConvertU16ToDecimal_11();
  ConvertU16ToDecimal_12();
  Printf_1();
  Printf_2();
  Printf_3();
  Printf_4();
  Printf_5();
  Printf_6();
  Printf_7();
  Printf_8();
  Printf_9();
  Printf_10();
  Printf_11();
  Printf_12();
  Printf_13();
  Printf_14();
  Printf_15();
  Printf_16();
  Printf_17();
  Printf_18();
  Printf_19();
  Printf_20();
  Printf_21();
  Printf_22();
  Printf_23();
  Printf_24();
  Printf_25();
  Printf_26();
  Printf_27();
  Printf_28();
  Printf_29();
  Printf_30();
  Printf_31();
  Printf_32();
  Printf_33();
  Printf_34();
  Printf_35();
  Printf_36();
  Printf_37();
  Printf_38();
  Printf_39();
  Printf_40();
  Printf_41();
  Printf_42();
  Printf_43();
}

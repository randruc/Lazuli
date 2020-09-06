/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Lazuli kernel unit tests.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file contains unit tests to test the kernel, along with a
 * micro-framework to perform unit tests on the target machine.
 */

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include <Lazuli/common.h>
#include <Lazuli/list.h>

#include <Lazuli/sys/arch/AVR/usart.h>
#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/printf.h>
#include <Lazuli/sys/scheduler.h>

DEPENDENCY_ON_MODULE(SERIAL);
DEPENDENCY_ON_MODULE(PRINTF);

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

/**
 * Test if two strings are equal.
 *
 * @param s1 The first string to test.
 * @param s2 The second string to test.
 *
 * @return true if the 2 strings are the same, else false.
 */
static bool
StringsAreEqual(const char * const s1, const char * const s2);

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

typedef struct {
  char c;
  Lz_LinkedListElement element;
}TestListItem;

UNIT_TEST(List_Append_1)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  Lz_LinkedListElement *item;

  ASSERT(List_IsEmpty(&linkedList));

  List_Append(&linkedList, &a.element);
  List_Append(&linkedList, &b.element);
  List_Append(&linkedList, &c.element);
  List_Append(&linkedList, &d.element);
  List_Append(&linkedList, &e.element);

  ASSERT(!List_IsEmpty(&linkedList));
  item = List_PickFirst(&linkedList);
  ASSERT(NULL != item);
  ASSERT('A' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList));
  item = List_PickFirst(&linkedList);
  ASSERT(NULL != item);
  ASSERT('B' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList));
  item = List_PickFirst(&linkedList);
  ASSERT(NULL != item);
  ASSERT('C' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList));
  item = List_PickFirst(&linkedList);
  ASSERT(NULL != item);
  ASSERT('D' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList));
  item = List_PickFirst(&linkedList);
  ASSERT(NULL != item);
  ASSERT('E' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(List_IsEmpty(&linkedList));
  ASSERT(NULL == List_PointFirst(&linkedList));
}

UNIT_TEST(List_ForEach_1)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  TestListItem *item;
  char testChar = 'A';

  ASSERT(List_IsEmpty(&linkedList));

  List_Append(&linkedList, &a.element);
  List_Append(&linkedList, &b.element);
  List_Append(&linkedList, &c.element);
  List_Append(&linkedList, &d.element);
  List_Append(&linkedList, &e.element);

  ASSERT(!List_IsEmpty(&linkedList));

  List_ForEach(&linkedList, TestListItem, item, element) {
    ASSERT(NULL != item);
    ASSERT(testChar == item->c);
    ++testChar;
  }

  ASSERT(!List_IsEmpty(&linkedList));
}

UNIT_TEST(List_ForEach_2)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem *item;

  ASSERT(List_IsEmpty(&linkedList));

  List_ForEach(&linkedList, TestListItem, item, element) {
    ASSERT(false);
  }

  ASSERT(List_IsEmpty(&linkedList));
}

UNIT_TEST(List_RemovableForEach_1)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  TestListItem *item;
  Lz_LinkedListElement *iterator;
  char testChar = 'A';

  ASSERT(List_IsEmpty(&linkedList));

  List_Append(&linkedList, &a.element);
  List_Append(&linkedList, &b.element);
  List_Append(&linkedList, &c.element);
  List_Append(&linkedList, &d.element);
  List_Append(&linkedList, &e.element);

  ASSERT(!List_IsEmpty(&linkedList));

  List_RemovableForEach(&linkedList, TestListItem, item, element, iterator) {
    ASSERT(NULL != item);
    ASSERT(testChar == item->c);
    ++testChar;
  }

  ASSERT(!List_IsEmpty(&linkedList));
}

UNIT_TEST(List_RemovableForEach_2)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem *item;
  Lz_LinkedListElement *iterator;

  ASSERT(List_IsEmpty(&linkedList));

  List_RemovableForEach(&linkedList, TestListItem, item, element, iterator) {
    ASSERT(false);
  }

  ASSERT(List_IsEmpty(&linkedList));
}

UNIT_TEST(List_RemovableForEach_3)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  TestListItem *item;
  Lz_LinkedListElement *iterator;
  Lz_LinkedListElement *pointedElement;
  char testChar = 'A';

  ASSERT(List_IsEmpty(&linkedList));

  List_Append(&linkedList, &a.element);
  List_Append(&linkedList, &b.element);
  List_Append(&linkedList, &c.element);
  List_Append(&linkedList, &d.element);
  List_Append(&linkedList, &e.element);

  ASSERT(!List_IsEmpty(&linkedList));

  List_RemovableForEach(&linkedList, TestListItem, item, element, iterator) {
    ASSERT(NULL != item);
    ASSERT(testChar == item->c);
    ++testChar;

    if ('B' == item->c || 'D' == item->c) {
      iterator = List_Remove(&linkedList, &item->element);
    }
  }

  pointedElement = List_PointElementAt(&linkedList, 0);
  ASSERT('A' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 1);
  ASSERT('C' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 2);
  ASSERT('E' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 3);
  ASSERT(NULL == pointedElement);

  ASSERT(!List_IsEmpty(&linkedList));
}

UNIT_TEST(List_RemovableForEach_4)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  TestListItem *item;
  Lz_LinkedListElement *iterator;
  Lz_LinkedListElement *pointedElement;
  char testChar = 'A';

  ASSERT(List_IsEmpty(&linkedList));

  List_Append(&linkedList, &a.element);
  List_Append(&linkedList, &b.element);
  List_Append(&linkedList, &c.element);
  List_Append(&linkedList, &d.element);
  List_Append(&linkedList, &e.element);

  ASSERT(!List_IsEmpty(&linkedList));

  List_RemovableForEach(&linkedList, TestListItem, item, element, iterator) {
    ASSERT(NULL != item);
    ASSERT(testChar == item->c);
    ++testChar;

    if ('B' == item->c || 'C' == item->c || 'D' == item->c) {
      iterator = List_Remove(&linkedList, &item->element);
    }
  }

  pointedElement = List_PointElementAt(&linkedList, 0);
  ASSERT('A' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 1);
  ASSERT('E' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 2);
  ASSERT(NULL == pointedElement);

  ASSERT(!List_IsEmpty(&linkedList));
}

UNIT_TEST(List_RemovableForEach_5)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  TestListItem *item;
  Lz_LinkedListElement *iterator;
  Lz_LinkedListElement *pointedElement;
  char testChar = 'A';

  ASSERT(List_IsEmpty(&linkedList));

  List_Append(&linkedList, &a.element);
  List_Append(&linkedList, &b.element);
  List_Append(&linkedList, &c.element);
  List_Append(&linkedList, &d.element);
  List_Append(&linkedList, &e.element);

  ASSERT(!List_IsEmpty(&linkedList));

  List_RemovableForEach(&linkedList, TestListItem, item, element, iterator) {
    ASSERT(NULL != item);
    ASSERT(testChar == item->c);
    ++testChar;

    if ('A' == item->c) {
      iterator = List_Remove(&linkedList, &item->element);
    }
  }

  pointedElement = List_PointElementAt(&linkedList, 0);
  ASSERT('B' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 1);
  ASSERT('C' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 2);
  ASSERT('D' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 3);
  ASSERT('E' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 4);
  ASSERT(NULL == pointedElement);

  ASSERT(!List_IsEmpty(&linkedList));
}

UNIT_TEST(List_RemovableForEach_6)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  TestListItem *item;
  Lz_LinkedListElement *iterator;
  Lz_LinkedListElement *pointedElement;
  char testChar = 'A';

  ASSERT(List_IsEmpty(&linkedList));

  List_Append(&linkedList, &a.element);
  List_Append(&linkedList, &b.element);
  List_Append(&linkedList, &c.element);
  List_Append(&linkedList, &d.element);
  List_Append(&linkedList, &e.element);

  ASSERT(!List_IsEmpty(&linkedList));

  List_RemovableForEach(&linkedList, TestListItem, item, element, iterator) {
    ASSERT(NULL != item);
    ASSERT(testChar == item->c);
    ++testChar;

    if ('A' == item->c || 'B' == item->c) {
      iterator = List_Remove(&linkedList, &item->element);
    }
  }

  pointedElement = List_PointElementAt(&linkedList, 0);
  ASSERT('C' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 1);
  ASSERT('D' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 2);
  ASSERT('E' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 3);
  ASSERT(NULL == pointedElement);

  ASSERT(!List_IsEmpty(&linkedList));
}

UNIT_TEST(List_RemovableForEach_7)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  TestListItem *item;
  Lz_LinkedListElement *iterator;
  Lz_LinkedListElement *pointedElement;
  char testChar = 'A';

  ASSERT(List_IsEmpty(&linkedList));

  List_Append(&linkedList, &a.element);
  List_Append(&linkedList, &b.element);
  List_Append(&linkedList, &c.element);
  List_Append(&linkedList, &d.element);
  List_Append(&linkedList, &e.element);

  ASSERT(!List_IsEmpty(&linkedList));

  List_RemovableForEach(&linkedList, TestListItem, item, element, iterator) {
    ASSERT(NULL != item);
    ASSERT(testChar == item->c);
    ++testChar;

    if ('D' == item->c) {
      iterator = List_Remove(&linkedList, &item->element);
    }
  }

  pointedElement = List_PointElementAt(&linkedList, 0);
  ASSERT('A' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 1);
  ASSERT('B' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 2);
  ASSERT('C' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 3);
  ASSERT('E' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 4);
  ASSERT(NULL == pointedElement);

  ASSERT(!List_IsEmpty(&linkedList));
}

UNIT_TEST(List_RemovableForEach_8)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  TestListItem *item;
  Lz_LinkedListElement *iterator;
  Lz_LinkedListElement *pointedElement;
  char testChar = 'A';

  ASSERT(List_IsEmpty(&linkedList));

  List_Append(&linkedList, &a.element);
  List_Append(&linkedList, &b.element);
  List_Append(&linkedList, &c.element);
  List_Append(&linkedList, &d.element);
  List_Append(&linkedList, &e.element);

  ASSERT(!List_IsEmpty(&linkedList));

  List_RemovableForEach(&linkedList, TestListItem, item, element, iterator) {
    ASSERT(NULL != item);
    ASSERT(testChar == item->c);
    ++testChar;

    if ('E' == item->c) {
      iterator = List_Remove(&linkedList, &item->element);
    }
  }

  pointedElement = List_PointElementAt(&linkedList, 0);
  ASSERT('A' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 1);
  ASSERT('B' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 2);
  ASSERT('C' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 3);
  ASSERT('D' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 4);
  ASSERT(NULL == pointedElement);

  ASSERT(!List_IsEmpty(&linkedList));
}

UNIT_TEST(List_RemovableForEach_9)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  TestListItem *item;
  Lz_LinkedListElement *iterator;
  Lz_LinkedListElement *pointedElement;
  char testChar = 'A';

  ASSERT(List_IsEmpty(&linkedList));

  List_Append(&linkedList, &a.element);
  List_Append(&linkedList, &b.element);
  List_Append(&linkedList, &c.element);
  List_Append(&linkedList, &d.element);
  List_Append(&linkedList, &e.element);

  ASSERT(!List_IsEmpty(&linkedList));

  List_RemovableForEach(&linkedList, TestListItem, item, element, iterator) {
    ASSERT(NULL != item);
    ASSERT(testChar == item->c);
    ++testChar;

    if ('D' == item->c || 'E' == item->c) {
      iterator = List_Remove(&linkedList, &item->element);
    }
  }

  pointedElement = List_PointElementAt(&linkedList, 0);
  ASSERT('A' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 1);
  ASSERT('B' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 2);
  ASSERT('C' == CONTAINER_OF(pointedElement, element, TestListItem)->c);

  pointedElement = List_PointElementAt(&linkedList, 3);
  ASSERT(NULL == pointedElement);

  ASSERT(!List_IsEmpty(&linkedList));
}

UNIT_TEST(List_RemovableForEach_10)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  TestListItem *item;
  Lz_LinkedListElement *iterator;
  char testChar = 'A';

  ASSERT(List_IsEmpty(&linkedList));

  List_Append(&linkedList, &a.element);
  List_Append(&linkedList, &b.element);
  List_Append(&linkedList, &c.element);
  List_Append(&linkedList, &d.element);
  List_Append(&linkedList, &e.element);

  ASSERT(!List_IsEmpty(&linkedList));

  List_RemovableForEach(&linkedList, TestListItem, item, element, iterator) {
    ASSERT(NULL != item);
    ASSERT(testChar == item->c);
    ++testChar;

    iterator = List_Remove(&linkedList, &item->element);
  }

  ASSERT(List_IsEmpty(&linkedList));
}

UNIT_TEST(List_Prepend_1)
{
  Lz_LinkedList linkedList = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  Lz_LinkedListElement *item;

  ASSERT(List_IsEmpty(&linkedList));

  List_Prepend(&linkedList, &a.element);
  List_Prepend(&linkedList, &b.element);
  List_Prepend(&linkedList, &c.element);

  ASSERT(!List_IsEmpty(&linkedList));
  item = List_PickFirst(&linkedList);
  ASSERT(NULL != item);
  ASSERT('C' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList));
  item = List_PickFirst(&linkedList);
  ASSERT(NULL != item);
  ASSERT('B' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList));
  item = List_PickFirst(&linkedList);
  ASSERT(NULL != item);
  ASSERT('A' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(List_IsEmpty(&linkedList));
  ASSERT(NULL == List_PointFirst(&linkedList));
}

UNIT_TEST(List_AppendList_1)
{
  Lz_LinkedList linkedList1 = LINKED_LIST_INIT;
  Lz_LinkedList linkedList2 = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  Lz_LinkedListElement *item;

  ASSERT(List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  List_Append(&linkedList1, &a.element);
  List_Append(&linkedList1, &b.element);
  List_Append(&linkedList1, &c.element);

  List_Append(&linkedList2, &d.element);
  List_Append(&linkedList2, &e.element);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(!List_IsEmpty(&linkedList2));

  List_AppendList(&linkedList1, &linkedList2);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  item = List_PickFirst(&linkedList1);
  ASSERT(NULL != item);
  ASSERT('A' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  item = List_PickFirst(&linkedList1);
  ASSERT(NULL != item);
  ASSERT('B' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  item = List_PickFirst(&linkedList1);
  ASSERT(NULL != item);
  ASSERT('C' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  item = List_PickFirst(&linkedList1);
  ASSERT(NULL != item);
  ASSERT('D' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  item = List_PickFirst(&linkedList1);
  ASSERT(NULL != item);
  ASSERT('E' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  ASSERT(NULL == List_PointFirst(&linkedList1));
  ASSERT(NULL == List_PointFirst(&linkedList2));
}

UNIT_TEST(List_AppendList_2)
{
  Lz_LinkedList linkedList1 = LINKED_LIST_INIT;
  Lz_LinkedList linkedList2 = LINKED_LIST_INIT;
  TestListItem d = { 'D', LINKED_LIST_ELEMENT_INIT };
  TestListItem e = { 'E', LINKED_LIST_ELEMENT_INIT };
  Lz_LinkedListElement *item;

  ASSERT(List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  List_Append(&linkedList2, &d.element);
  List_Append(&linkedList2, &e.element);

  ASSERT(List_IsEmpty(&linkedList1));
  ASSERT(!List_IsEmpty(&linkedList2));

  List_AppendList(&linkedList1, &linkedList2);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  item = List_PickFirst(&linkedList1);
  ASSERT(NULL != item);
  ASSERT('D' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  item = List_PickFirst(&linkedList1);
  ASSERT(NULL != item);
  ASSERT('E' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  ASSERT(NULL == List_PointFirst(&linkedList1));
  ASSERT(NULL == List_PointFirst(&linkedList2));
}

UNIT_TEST(List_AppendList_3)
{
  Lz_LinkedList linkedList1 = LINKED_LIST_INIT;
  Lz_LinkedList linkedList2 = LINKED_LIST_INIT;
  TestListItem a = { 'A', LINKED_LIST_ELEMENT_INIT };
  TestListItem b = { 'B', LINKED_LIST_ELEMENT_INIT };
  TestListItem c = { 'C', LINKED_LIST_ELEMENT_INIT };
  Lz_LinkedListElement *item;

  ASSERT(List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  List_Append(&linkedList1, &a.element);
  List_Append(&linkedList1, &b.element);
  List_Append(&linkedList1, &c.element);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  List_AppendList(&linkedList1, &linkedList2);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  item = List_PickFirst(&linkedList1);
  ASSERT(NULL != item);
  ASSERT('A' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  item = List_PickFirst(&linkedList1);
  ASSERT(NULL != item);
  ASSERT('B' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(!List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  item = List_PickFirst(&linkedList1);
  ASSERT(NULL != item);
  ASSERT('C' == CONTAINER_OF(item, element, TestListItem)->c);

  ASSERT(List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  ASSERT(NULL == List_PointFirst(&linkedList1));
  ASSERT(NULL == List_PointFirst(&linkedList2));
}

UNIT_TEST(List_AppendList_4)
{
  Lz_LinkedList linkedList1 = LINKED_LIST_INIT;
  Lz_LinkedList linkedList2 = LINKED_LIST_INIT;

  ASSERT(List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  List_AppendList(&linkedList1, &linkedList2);

  ASSERT(List_IsEmpty(&linkedList1));
  ASSERT(List_IsEmpty(&linkedList2));

  ASSERT(NULL == List_PointFirst(&linkedList1));
  ASSERT(NULL == List_PointFirst(&linkedList2));
}

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

UNIT_TEST(Printf_1)
{
  int total = printf("2019:%u\r\n", 2019);

  ASSERT(11 == total);
}

UNIT_TEST(Printf_2)
{
  int total = printf("-1205:%d\r\n", -1205);

  ASSERT(13 == total);
}

UNIT_TEST(Printf_3)
{
  int total = printf("0:%d\r\n", 0);

  ASSERT(5 == total);
}

UNIT_TEST(Printf_4)
{
  int total = printf("1:%d\r\n", 1);

  ASSERT(5 == total);
}

UNIT_TEST(Printf_5)
{
  int total = printf("65535:%u\r\n", 65535u);

  ASSERT(13 == total);
}

UNIT_TEST(Printf_6)
{
  int total = printf("-1:%d\r\n", -1);

  ASSERT(7 == total);
}

UNIT_TEST(Printf_7)
{
  int total = printf("48620:%u\r\n", 48620u);

  ASSERT(13 == total);
}

UNIT_TEST(Printf_8)
{
  int total = printf("0000048620:%010u\r\n", 48620u);

  ASSERT(23 == total);
}

UNIT_TEST(Printf_9)
{
  int total = printf("25631:%01u\r\n", 25631u);

  ASSERT(13 == total);
}

UNIT_TEST(Printf_10)
{
  int total = printf("228:%03u\r\n", 228u);

  ASSERT(9 == total);
}

UNIT_TEST(Printf_11)
{
  int total = printf("-1715:%03d\r\n", -1715);

  ASSERT(13 == total);
}

UNIT_TEST(Printf_12)
{
  int total = printf("-004:%04d\r\n", -4);

  ASSERT(11 == total);
}

UNIT_TEST(Printf_13)
{
  int total = printf("-563:%02d\r\n", -563);

  ASSERT(11 == total);
}

UNIT_TEST(Printf_14)
{
  int total = printf("00001963:%08d\r\n", 1963);

  ASSERT(19 == total);
}

UNIT_TEST(Printf_15)
{
  int total = printf("065535:%06u\r\n", 65535u);

  ASSERT(15 == total);
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
  LoadFromProgmem_2,
  List_Append_1,
  List_ForEach_1,
  List_ForEach_2,
  List_RemovableForEach_1,
  List_RemovableForEach_2,
  List_RemovableForEach_3,
  List_RemovableForEach_4,
  List_RemovableForEach_5,
  List_RemovableForEach_6,
  List_RemovableForEach_7,
  List_RemovableForEach_8,
  List_RemovableForEach_9,
  List_RemovableForEach_10,
  List_Prepend_1,
  List_AppendList_1,
  List_AppendList_2,
  List_AppendList_3,
  List_AppendList_4,
  Variadic_Function_1,
  Variadic_Function_2,
  Variadic_Function_3,
  Variadic_Function_4,
  Variadic_Function_Passtrough_5,
  ConvertU16ToDecimal_1,
  ConvertU16ToDecimal_2,
  ConvertU16ToDecimal_3,
  ConvertU16ToDecimal_4,
  ConvertU16ToDecimal_5,
  ConvertU16ToDecimal_6,
  ConvertU16ToDecimal_7,
  ConvertU16ToDecimal_8,
  ConvertU16ToDecimal_9,
  ConvertU16ToDecimal_10,
  ConvertU16ToDecimal_11,
  Printf_1,
  Printf_2,
  Printf_3,
  Printf_4,
  Printf_5,
  Printf_6,
  Printf_7,
  Printf_8,
  Printf_9,
  Printf_10,
  Printf_11,
  Printf_12,
  Printf_13,
  Printf_14,
  Printf_15
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

static bool
StringsAreEqual(const char * const s1, const char * const s2)
{
  size_t i = 0;

  while ('\0' != s1[i]) {
    if (s1[i] != s2[i]) {
      return false;
    }

    ++i;
  }

  if ('\0' != s2[i]) {
    return false;
  }

  return true;
}

/**
 * Activate serial transmission.
 */
static void
EnableSerialTransmission(void) {
  Lz_SerialConfiguration serialConfiguration;

  Lz_Serial_GetConfiguration(&serialConfiguration);
  serialConfiguration.enableFlags = LZ_SERIAL_ENABLE_TRANSMIT;
  serialConfiguration.speed = LZ_SERIAL_SPEED_19200;
  Lz_Serial_SetConfiguration(&serialConfiguration);
}

int
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

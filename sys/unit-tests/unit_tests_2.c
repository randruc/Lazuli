/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Lazuli kernel unit tests part 2.
 * @copyright 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file contains unit tests to test the kernel.
 */

#include "unit_tests_common.h"

#include <stdint.h>
#include <stdio.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>
#include <Lazuli/list.h>

#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/scheduler.h>

DEPENDENCY_ON_MODULE(DIVISION);
DEPENDENCY_ON_MODULE(SERIAL);

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

UNIT_TEST(Division_1)
{
  const unsigned int numerator = 15;
  const unsigned int denominator = 1;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(15 == d.quotient);
  ASSERT(0 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_2)
{
  const unsigned int numerator = 64683;
  const unsigned int denominator = 173;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(373 == d.quotient);
  ASSERT(154 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_3)
{
  const unsigned int numerator = 21447;
  const unsigned int denominator = 325;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(65 == d.quotient);
  ASSERT(322 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_4)
{
  const unsigned int numerator = 51809;
  const unsigned int denominator = 484;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(107 == d.quotient);
  ASSERT(21 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_5)
{
  const unsigned int numerator = 30545;
  const unsigned int denominator = 453;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(67 == d.quotient);
  ASSERT(194 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_6)
{
  const unsigned int numerator = 54521;
  const unsigned int denominator = 462;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(118 == d.quotient);
  ASSERT(5 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_7)
{
  const unsigned int numerator = 54955;
  const unsigned int denominator = 261;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(210 == d.quotient);
  ASSERT(145 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_8)
{
  const unsigned int numerator = 53284;
  const unsigned int denominator = 525;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(101 == d.quotient);
  ASSERT(259 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_9)
{
  const unsigned int numerator = 29840;
  const unsigned int denominator = 205;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(145 == d.quotient);
  ASSERT(115 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_10)
{
  const unsigned int numerator = 57526;
  const unsigned int denominator = 448;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(128 == d.quotient);
  ASSERT(182 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_11)
{
  const unsigned int numerator = 11047;
  const unsigned int denominator = 582;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(18 == d.quotient);
  ASSERT(571 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_12)
{
  const unsigned int numerator = 63677;
  const unsigned int denominator = 165;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(385 == d.quotient);
  ASSERT(152 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_13)
{
  const unsigned int numerator = 31447;
  const unsigned int denominator = 180;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(174 == d.quotient);
  ASSERT(127 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_14)
{
  const unsigned int numerator = 27978;
  const unsigned int denominator = 153;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(182 == d.quotient);
  ASSERT(132 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_15)
{
  const unsigned int numerator = 0;
  const unsigned int denominator = 153;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(0 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_16)
{
  const unsigned int numerator = 65535;
  const unsigned int denominator = 1;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(65535 == d.quotient);
  ASSERT(0 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_17)
{
  const unsigned int numerator = 0;
  const unsigned int denominator = 65535;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(0 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_18)
{
  const unsigned int numerator = 65535;
  const unsigned int denominator = 65534;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(1 == d.quotient);
  ASSERT(1 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_19)
{
  const unsigned int numerator = 65534;
  const unsigned int denominator = 65535;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(65534 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_20)
{
  const unsigned int numerator = 183;
  const unsigned int denominator = 41573;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(183 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_21)
{
  const unsigned int numerator = 147;
  const unsigned int denominator = 40525;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(147 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_22)
{
  const unsigned int numerator = 209;
  const unsigned int denominator = 55084;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(209 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_23)
{
  const unsigned int numerator = 245;
  const unsigned int denominator = 23253;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(245 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_24)
{
  const unsigned int numerator = 221;
  const unsigned int denominator = 7062;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(221 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_25)
{
  const unsigned int numerator = 55;
  const unsigned int denominator = 58461;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(55 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_26)
{
  const unsigned int numerator = 184;
  const unsigned int denominator = 34725;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(184 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_27)
{
  const unsigned int numerator = 140;
  const unsigned int denominator = 670;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(140 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_28)
{
  const unsigned int numerator = 226;
  const unsigned int denominator = 49648;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(226 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_29)
{
  const unsigned int numerator = 247;
  const unsigned int denominator = 8382;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(247 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_32)
{
  const unsigned int numerator = 1;
  const unsigned int denominator = 7000;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(1 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_33)
{
  const unsigned int numerator = 65535;
  const unsigned int denominator = 65535;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(1 == d.quotient);
  ASSERT(0 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_34)
{
  const unsigned int numerator = 2;
  const unsigned int denominator = 2;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(1 == d.quotient);
  ASSERT(0 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

UNIT_TEST(Division_35)
{
  const unsigned int numerator = 0;
  const unsigned int denominator = 65535;

  U16DivisionResult d = Arch_Divide_U16(numerator, denominator);

  printf("D:num=%u den=%u quot=%u rem=%u" LZ_CONFIG_SERIAL_NEWLINE,
         numerator,
         denominator,
         d.quotient,
         d.remainder);

  ASSERT(0 == d.quotient);
  ASSERT(0 == d.remainder);
  ASSERT(numerator == denominator * d.quotient + d.remainder);
}

void
ExecuteTests(void)
{
  GlobalEnableDisableInterrupts();
  GlobalEnableDisableInterruptsWithStatus_1();
  GlobalEnableDisableInterruptsWithStatus_2();
  ReverseBytesOfFunctionPointer_1();
  ReverseBytesOfFunctionPointer_2();
  ReverseBytesOfFunctionPointer_3();
  LoadU8FromProgmem_1();
  LoadU8FromProgmem_2();
  LoadPointerFromProgmem_1();
  LoadPointerFromProgmem_2();
  LoadFunctionPointerFromProgmem_1();
  LoadFunctionPointerFromProgmem_2();
  LoadU16FromProgmem_1();
  LoadU16FromProgmem_2();
  LoadFromProgmem_1();
  LoadFromProgmem_2();
  List_Append_1();
  List_ForEach_1();
  List_ForEach_2();
  List_RemovableForEach_1();
  List_RemovableForEach_2();
  List_RemovableForEach_3();
  List_RemovableForEach_4();
  List_RemovableForEach_5();
  List_RemovableForEach_6();
  List_RemovableForEach_7();
  List_RemovableForEach_8();
  List_RemovableForEach_9();
  List_RemovableForEach_10();
  List_Prepend_1();
  List_AppendList_1();
  List_AppendList_2();
  List_AppendList_3();
  List_AppendList_4();
  Division_1();
  Division_2();
  Division_3();
  Division_4();
  Division_5();
  Division_6();
  Division_7();
  Division_8();
  Division_9();
  Division_10();
  Division_11();
  Division_12();
  Division_13();
  Division_14();
  Division_15();
  Division_16();
  Division_17();
  Division_18();
  Division_19();
  Division_20();
  Division_21();
  Division_22();
  Division_23();
  Division_24();
  Division_25();
  Division_26();
  Division_27();
  Division_28();
  Division_29();
  Division_32();
  Division_33();
  Division_34();
  Division_35();
}

/**
 * @file src/kern/list.c
 *
 * This file describes the operations related to lists.
 */

#include <Lazuli/common.h>
#include <Lazuli/list.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/memory.h>

void
List_Append(LinkedList * const linkedList, Lz_LinkedListElement * const item)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList || NULL == item) {
      return;
    }
  }

  item->next = NULL;

  if (NULL == linkedList->first) {
    item->prev = NULL;
    linkedList->first = item;
    linkedList->last = item;

    return;
  }

  item->prev = linkedList->last;
  linkedList->last->next = item;
  linkedList->last = item;
}

void
List_Prepend(LinkedList * const linkedList, Lz_LinkedListElement * const item)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList || NULL == item) {
      return;
    }
  }

  item->prev = NULL;

  if (NULL == linkedList->first) {
    item->next = NULL;
    linkedList->first = item;
    linkedList->last = item;

    return;
  }

  item->next = linkedList->first;
  linkedList->first->prev = item;
  linkedList->first = item;
}

void
List_AppendList(LinkedList * const linkedListDestination,
                LinkedList * const linkedListToMove)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedListDestination || NULL == linkedListToMove) {
      return;
    }
  }

  if (NULL == linkedListToMove->first) {
    return;
  }

  if (NULL == linkedListDestination->first) {
    linkedListDestination->first = linkedListToMove->first;
  } else {
    linkedListDestination->last->next = linkedListToMove->first;
    linkedListToMove->first->prev = linkedListDestination->last;
  }

  linkedListDestination->last = linkedListToMove->last;

  linkedListToMove->first = NULL;
  linkedListToMove->last = NULL;
}

Lz_LinkedListElement *
List_PickFirst(LinkedList * const linkedList)
{
  Lz_LinkedListElement *item;

  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList) {
      return NULL;
    }
  }

  if (NULL == linkedList->first) {
    return NULL;
  }

  item = linkedList->first;

  linkedList->first = linkedList->first->next;

  if (NULL == linkedList->first) {
    linkedList->last = NULL;
  } else {
    linkedList->first->prev = NULL;
  }

  item->next = NULL;
  item->prev = NULL;

  return item;
}

Lz_LinkedListElement *
List_PointFirst(const LinkedList * const linkedList)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList) {
      return NULL;
    }
  }

  return linkedList->first;
}

bool
List_IsEmpty(const LinkedList * const linkedList)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList) {
      return true;
    }
  }

  return (NULL == linkedList->first);
}

void
List_InsertAfter(LinkedList * const linkedList,
                 Lz_LinkedListElement * const listItem,
                 Lz_LinkedListElement * const itemToInsert)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList || NULL == listItem || NULL == itemToInsert) {
      return;
    }
  }

  if (linkedList->last == listItem) {
    linkedList->last = itemToInsert;
  }

  itemToInsert->next = listItem->next;
  itemToInsert->prev = listItem;
  listItem->next = itemToInsert;
}

void
List_InsertBefore(LinkedList * const linkedList,
                  Lz_LinkedListElement * const listItem,
                  Lz_LinkedListElement * const itemToInsert)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList || NULL == listItem || NULL == itemToInsert) {
      return;
    }
  }

  if (linkedList->first == listItem) {
    linkedList->first = itemToInsert;
  }

  itemToInsert->next = listItem;
  itemToInsert->prev = listItem->prev;
  listItem->prev = itemToInsert;
}

bool
List_IsLastElement(const LinkedList * const linkedList,
                   const Lz_LinkedListElement * const item)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList || NULL == item) {
      return true;
    }
  }

  return linkedList->last == item;
}

bool
List_IsFirstElement(const LinkedList * const linkedList,
                    const Lz_LinkedListElement * const item)
{
  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList || NULL == item) {
      return true;
    }
  }

  return linkedList->first == item;
}

void
List_InitLinkedList(LinkedList * const linkedList)
{
  const LinkedList linkedListInit = LINKED_LIST_INIT;

  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList) {
      return;
    }
  }

  Memory_Copy(&linkedListInit, linkedList, sizeof(linkedListInit));
}

void
List_InitLinkedListElement(Lz_LinkedListElement * const item)
{
  const Lz_LinkedListElement linkedListElementInit = LINKED_LIST_ELEMENT_INIT;

  if (LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == item) {
      return;
    }
  }

  Memory_Copy(&linkedListElementInit, item, sizeof(linkedListElementInit));
}

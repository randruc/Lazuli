/**
 * @file src/kern/list.c
 *
 * This file describes the operations related to lists.
 */

#include <Lazuli/common.h>

#include <Lazuli/sys/list.h>
#include <Lazuli/sys/config.h>

void
List_Append(LinkedList * const linkedList, LinkedListElement * const item)
{
  if (CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList || NULL == item) {
      return;
    }
  }

  item->next = NULL;

  if (NULL == linkedList->first) {
    linkedList->first = item;
    linkedList->last = item;

    return;
  }

  linkedList->last->next = item;
  linkedList->last = item;
}

void
List_Prepend(LinkedList * const linkedList, LinkedListElement * const item)
{
  if (CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList || NULL == item) {
      return;
    }
  }

  if (NULL == linkedList->first) {
    item->next = NULL;
    linkedList->first = item;
    linkedList->last = item;

    return;
  }

  item->next = linkedList->first;
  linkedList->first = item;
}

void
List_AppendList(LinkedList * const linkedListDestination,
                LinkedList * const linkedListToMove)
{
  if (CHECK_NULL_PARAMETERS_IN_LISTS) {
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
  }

  linkedListDestination->last = linkedListToMove->last;

  linkedListToMove->first = NULL;
  linkedListToMove->last = NULL;
}

LinkedListElement *
List_PickFirst(LinkedList * const linkedList)
{
  LinkedListElement *item;

  if (CHECK_NULL_PARAMETERS_IN_LISTS) {
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
  }

  item->next = NULL;

  return item;
}

bool
List_IsEmpty(LinkedList * const linkedList)
{
  if (CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList) {
      return true;
    }
  }

  return (NULL == linkedList->first);
}

void
List_InsertAfter(LinkedList * const linkedList,
                 LinkedListElement * const listItem,
                 LinkedListElement * const itemToInsert)
{
  if (CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList || NULL == listItem || NULL == itemToInsert) {
      return;
    }
  }

  if (linkedList->last == listItem) {
    linkedList->last = itemToInsert;
  }

  itemToInsert->next = listItem->next;
  listItem->next = itemToInsert;
}

bool
List_IsLastElement(LinkedList * const linkedList,
                   LinkedListElement * const item)
{
  if (CHECK_NULL_PARAMETERS_IN_LISTS) {
    if (NULL == linkedList || NULL == item) {
      return true;
    }
  }

  return linkedList->last == item;
}

/**
 * @file.c
 *
 * This file describes the operations related to lists.
 */

#include <Lazuli/common.h>
#include <Lazuli/sys/list.h>

void
List_Append(LinkedListHead * const listHead, LinkedListElement * const item)
{
  LinkedListElement *current;

  if (NULL == listHead || NULL == item) {
    return;
  }

  if (NULL == listHead->first) {
    listHead->first = item;

    return;
  }

  current = listHead->first;

  while (NULL != current->next) {
    current = current->next;
  }

  current->next = item;
}

LinkedListElement *
List_PickFirst(LinkedListHead * const listHead)
{
  LinkedListElement *firstElement;

  if (NULL == listHead) {
    return NULL;
  }

  firstElement = listHead->first;
  if (NULL == firstElement) {
    return NULL;
  }

  listHead->first = firstElement->next;

  return firstElement;
}

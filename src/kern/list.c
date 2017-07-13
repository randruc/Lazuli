/**
 * @file.c
 *
 * This file describes the operations related to lists.
 */

#include <Lazuli/common.h>
#include <Lazuli/sys/list.h>

void
List_Append(LinkedList * const linkedList, LinkedListElement * const item)
{
  if (NULL == linkedList || NULL == item) {
    return;
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

LinkedListElement *
List_PickFirst(LinkedList * const linkedList)
{
  LinkedListElement *item;

  if (NULL == linkedList) {
    return NULL;
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

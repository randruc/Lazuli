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

void
List_AppendList(LinkedList * const linkedListDestination,
                LinkedList * const linkedListToMove)
{
  if (NULL == linkedListDestination || NULL == linkedListToMove) {
    return;
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

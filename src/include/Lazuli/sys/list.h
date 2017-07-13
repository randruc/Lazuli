/**
 * @file list.h
 *
 * Describes types and function related to linked lists.
 */

#ifndef LZ_SYS_LIST_H
#define LZ_SYS_LIST_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents an element of a simply linked list.
 */
typedef struct _LinkedListElement {
  /** A pointer to the next element on the list */
  struct _LinkedListElement *next;
}LinkedListElement;

/**
 * Represents the main container for simply linked elements.
 */
typedef struct {
  /** A pointer to the first element of the linked list */
  LinkedListElement *first;

  /** A pointer to the last element of the linked list */
  LinkedListElement *last;
}LinkedList;

/**
 * Append a LinkedListElement to the end of an existing simply linked list.
 *
 * @param linkedList A pointer to the linked list head.
 * @param item A pointer to the item to append to the list.
 */
void
List_Append(LinkedList * const linkedList, LinkedListElement * const item);

/**
 * Return the first element of an existing simply linked list.
 *
 * @param linkedList A pointer to the linked list head.
 *
 * @return A pointer to the the first element of the list.
 */
LinkedListElement *
List_PickFirst(LinkedList * const linkedList);

_EXTERN_C_DECL_END

#endif /* LZ_SYS_LIST_H */

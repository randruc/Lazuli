/**
 * @file list.h
 *
 * Describes types and function related to linked lists.
 */

#ifndef LZ_LIST_H
#define LZ_LIST_H

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
 * Represents the head of a simply linked list.
 */
typedef struct {
  /** A pointer to the first element of the linked list */
  LinkedListElement *first;
}LinkedListHead;

/**
 * Append a LinkedListElement to the end of an existing simply linked list.
 *
 * @param listHead A pointer to the linked list head.
 * @param item A pointer to the item to append to the list.
 */
void
List_Append(LinkedListHead * const listHead, LinkedListElement * const item);

/**
 * Return the first element of an existing simply linked list.
 *
 * @param listHead A pointer to the linked list head.
 *
 * @return A pointer to the the first element of the list.
 */
LinkedListElement *
List_PickFirst(LinkedListHead * const listHead);

_EXTERN_C_DECL_END

#endif /* LZ_LIST_H */

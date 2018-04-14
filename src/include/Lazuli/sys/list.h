/**
 * @file src/include/Lazuli/sys/list.h
 *
 * Describes types and functions related to linked lists.
 */

#ifndef LZ_SYS_LIST_H
#define LZ_SYS_LIST_H

#include <Lazuli/common.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents an element of a singly linked list.
 */
typedef struct _LinkedListElement {
  /** A pointer to the next element on the list */
  struct _LinkedListElement *next;
}LinkedListElement;

/**
 * Represents the main container for singly linked elements.
 */
typedef struct {
  /** A pointer to the first element of the linked list */
  LinkedListElement *first;

  /** A pointer to the last element of the linked list */
  LinkedListElement *last;
}LinkedList;

/**
 * Define the initialization value for the type LinkedList.
 */
#define LINKED_LIST_INIT { NULL, NULL }

/**
 * Append a LinkedListElement to the end of an existing LinkedList.
 *
 * @param linkedList A pointer to the linked list head.
 * @param item A pointer to the item to append to the list.
 */
void
List_Append(LinkedList * const linkedList, LinkedListElement * const item);

/**
 * Insert a LinkedListElement as the first element of a LinkedList.
 *
 * @param linkedList A pointer to the linked list head.
 * @param item A pointer to the item to prepend to the list.
 */
void
List_Prepend(LinkedList * const linkedList, LinkedListElement * const item);

/**
 * When treating a LinkedList as a stack, push an element on top of the stack.
 * This function can be used in conjunction with List_Pop() when treating a
 * LinkedList as a stack.
 * This is nothing more than a macro alias of the List_Prepend() function.
 *
 * @param linkedList A pointer to the linked list head.
 * @param item A pointer to the item to append to the list.
 */
#define List_Push(linkedList, item) List_Prepend(linkedList, item)

/**
 * Move the content of a LinkedList to the end of an existing LinkedList.
 *
 * @param linkedListDestination A pointer to the LinkedList on which to append.
 * @param linkedListToMove A pointer to the LinkedList to move.
 */
void
List_AppendList(LinkedList * const linkedListDestination,
                LinkedList * const linkedListToMove);

/**
 * Return the first element of an existing singly linked list.
 *
 * @param linkedList A pointer to the linked list head.
 *
 * @return A pointer to the the first element of the list, or NULL if:
 *         - The linkedList is empty
 *         - The parameter linkedList is NULL
 */
LinkedListElement *
List_PickFirst(LinkedList * const linkedList);

/**
 * When treating a LinkedList as a stack, pop the element on top af the stack,
 * and return a pointer to it.
 * This function can be used in conjunction with List_Push() when treating a
 * LinkedList as a stack.
 * This is nothing more than a macro alias of the List_PickFirst() function.
 *
 * @param linkedList A pointer to the linked list head.
 *
 * @return A pointer to the pop'd element, or NULL if:
 *         - The linkedList is empty
 *         - The parameter linkedList is NULL
 */
#define List_Pop(linkedList) List_PickFirst(linkedList)

/**
 * Test if a LinkedList is empty.
 *
 * @param LinkedList A pointer to the LinkedList to test.
 *
 * @return true if:
 *         - The linkedList is empty
 *         - The parameter linkedList is NULL
 *         false if:
 *         - The linkedList contains at least 1 element
 */
bool
List_IsEmpty(LinkedList * const linkedList);

/**
 * Run through a LinkedList like a for loop.
 *
 * @param item A pointer to a LinkedListElement that will point to the current
 *             item of each loop. This pointer will never be NULL.
 * @param linkedList A pointer to the LinkedList to run through.
 *
 * @warning The linkedList pointer MUST NOT be NULL. No check is performed.
 */
#define List_ForEach(item, linkedList)          \
  for (item = (linkedList)->first; NULL != item->next; item = item->next)

/**
 * Insert an element after another in a LinkedList.
 *
 * @param linkedList A pointer to the LinkedList containing the element
 *                   listItem on which to insert after.
 * @param listItem A pointer to an element on which to insert after, already
 *                 present in a LinkedList.
 * @param itemToInsert A pointer to the item to insert in the list.
 *
 * @warning The listItem parameter MUST already be part of the LinkedList
 *          pointed to by parameter linkedList. No check is performed.
 */
void
List_InsertAfter(LinkedList * const linkedList,
                 LinkedListElement * const listItem,
                 LinkedListElement * const itemToInsert);

/**
 * Test if a LinkedListElement is the last entry of a LinkedList.
 *
 * @param linkedList A pointer to a LinkedList.
 * @param item A pointer to the LinkedListElement to test.
 *
 * @return true if:
 *         - The item is the last element of the linkedList
 *         - One of the parameters is NULL
 *         false if:
 *         - The item is not the last element of the linkedList
 *         - The item is not part of the likedList
 */
bool
List_IsLastElement(LinkedList * const linkedList,
                   LinkedListElement * const item);

_EXTERN_C_DECL_END

#endif /* LZ_SYS_LIST_H */

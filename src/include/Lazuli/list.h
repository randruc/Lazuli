/**
 * @file src/include/Lazuli/list.h
 * @brief Doubly linked lists interface.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * Describes types and functions related to doubly linked lists.
 */

#ifndef LAZULI_LIST_H
#define LAZULI_LIST_H

#include <Lazuli/common.h>
#include <Lazuli/sys/config.h>

_EXTERN_C_DECL_BEGIN

/**
 * Represents an element of a doubly linked list.
 */
typedef struct _LinkedListElement {
  /** A pointer to the next element in the list */
  struct _LinkedListElement *next;

  /** A pointer to the previous element in the list */
  struct _LinkedListElement *prev;
}LinkedListElement;

/**
 * Represents the main container for doubly linked elements.
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
 * Define the initialization value for the type LinkedListElement.
 */
#define LINKED_LIST_ELEMENT_INIT { NULL, NULL }

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
 * Move the content of a LinkedList to the end of an existing LinkedList.
 *
 * @param linkedListDestination A pointer to the LinkedList on which to append.
 * @param linkedListToMove A pointer to the LinkedList to move.
 */
void
List_AppendList(LinkedList * const linkedListDestination,
                LinkedList * const linkedListToMove);

/**
 * Return the first element of an existing linked list. This function drops the
 * first element of the list if it exists.
 *
 * @param linkedList A pointer to the linked list head.
 *
 * @return A pointer to the first element of the list, or _NULL_ if:
 *         - The @p linkedList is empty
 *         - The parameter @p linkedList is _NULL_
 */
LinkedListElement *
List_PickFirst(LinkedList * const linkedList);

/**
 * Return a pointer to the first element of an existing linked list. This
 * function does not drop the first element of the list.
 *
 * @param linkedList A pointer to an existing LinkedList.
 *
 * @return A pointer to the first element of the list, or _NULL_ if:
 *         - The @p linkedList is empty
 *         - The parameter @p linkedList is _NULL_
 */
LinkedListElement *
List_PointFirst(const LinkedList * const linkedList);

/**
 * Test if a LinkedList is empty.
 *
 * @param linkedList A pointer to the LinkedList to test.
 *
 * @return
 *         - _true_ if:
 *           + The @p linkedList is empty
 *           + The parameter @p linkedList is _NULL_
 *
 *         - _false_ if:
 *           + The @p linkedList contains at least 1 element
 */
bool
List_IsEmpty(const LinkedList * const linkedList);

/**
 * Run through a LinkedList like a for loop.
 *
 * If the list is empty, no loop is performed, and the execution will continue
 * after the foreach.
 *
 * With configuration option CHECK_NULL_PARAMETERS_IN_LISTS, this implementation
 * can also verify if the @p LINKEDLIST pointer is _NULL_. If so, the loop is
 * not run and the execution continues after the loop.
 *
 * @param LINKEDLIST A pointer to the LinkedList to run through.
 * @param ITEM A pointer to a LinkedListElement that will point to the current
 *             item of each loop turn. This pointer will never be _NULL_.
 *
 */
#define List_UntypedForEach(LINKEDLIST, ITEM)                           \
  if ((LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS && (NULL == (LINKEDLIST))) || \
      (NULL == (LINKEDLIST)->first))                                    \
    {}                                                                  \
  else                                                                  \
    for ((ITEM) = (LINKEDLIST)->first;                                  \
         NULL != (ITEM);                                                \
         (ITEM) = (ITEM)->next)

/**
 * Run through a LinkedList like a for loop.
 *
 * This foreach implementation is typed, so each loop turn will return a typed
 * pointer to the current loop element (ie. not a pointer to a raw
 * LinkedListElement).
 *
 * If the list is empty, no loop is performed, and the execution will continue
 * after the foreach.
 *
 * With configuration option CHECK_NULL_PARAMETERS_IN_LISTS, this implementation
 * can also verify if the @p LINKEDLIST pointer is _NULL_. If so, the loop is
 * not run and the execution continues after the loop.
 *
 * @param LINKEDLIST A pointer to the LinkedList to run through.
 * @param TYPE The real type of the list elements
 * @param ITEM A pointer to a LinkedListElement that will point to the current
 *             item of each loop turn. This pointer will never be _NULL_.
 * @param MEMBER The name of the member in @p TYPE which bears the
 *               LinkedListElement.
 */
#define List_ForEach(LINKEDLIST, TYPE, ITEM, MEMBER)                    \
  if ((LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS && (NULL == (LINKEDLIST))) || \
      (NULL == (LINKEDLIST)->first))                                    \
    {}                                                                  \
  else                                                                  \
    for ((ITEM) = CONTAINER_OF((LINKEDLIST)->first, MEMBER, TYPE);      \
         NULL != (ITEM);                                                \
         (ITEM) =                                                       \
           (NULL == ((ITEM)->MEMBER).next)                              \
           ? NULL                                                       \
           : CONTAINER_OF(((ITEM)->MEMBER).next, MEMBER, TYPE))

/**
 * Insert an element after another in a LinkedList.
 *
 * @param linkedList A pointer to the LinkedList containing the element
 *                   @p listItem on which to insert after.
 * @param listItem A pointer to an element on which to insert after, already
 *                 present in a LinkedList.
 * @param itemToInsert A pointer to the item to insert in the list.
 *
 * @warning The @p listItem parameter MUST already be part of the LinkedList
 *          pointed to by parameter @p linkedList. No check is performed.
 */
void
List_InsertAfter(LinkedList * const linkedList,
                 LinkedListElement * const listItem,
                 LinkedListElement * const itemToInsert);

/**
 * Insert an element before another in a LinkedList.
 *
 * @param linkedList A pointer to the LinkedList containing the element
 *                   @p listItem on which to insert before.
 * @param listItem A pointer to an element on which to insert before, already
 *                 present in a LinkedList.
 * @param itemToInsert A pointer to the item to insert in the list.
 *
 * @warning The @p listItem parameter MUST already be part of the LinkedList
 *          pointed to by parameter @p linkedList. No check is performed.
 */
void
List_InsertBefore(LinkedList * const linkedList,
                  LinkedListElement * const listItem,
                  LinkedListElement * const itemToInsert);

/**
 * Test if a LinkedListElement is the last entry of a LinkedList.
 *
 * @param linkedList A pointer to a LinkedList.
 * @param item A pointer to the LinkedListElement to test.
 *
 * @return
 *         - _true_ if:
 *             + The item is the last element of the linkedList
 *             + One of the parameters is _NULL_ (if checking for _NULL_
 *               parameters is enabled in the configuration)
 *
 *         - _false_ if:
 *             + The item is not the last element of the linkedList
 *             + The item is not part of the likedList
 */
bool
List_IsLastElement(const LinkedList * const linkedList,
                   const LinkedListElement * const item);

/**
 * Test if a LinkedListElement is the first entry of a LinkedList.
 *
 * @param linkedList A pointer to a LinkedList.
 * @param item A pointer to the LinkedListElement to test.
 *
 * @return
 *         - _true_ if:
 *             + The item is the first element of the linkedList
 *             + One of the parameters is _NULL_ (if checking for _NULL_
 *               parameters is enabled in the configuration)
 *
 *         - _false_ if:
 *             + The item is not the first element of the linkedList
 *             + The item is not part of the likedList
 */
bool
List_IsFirstElement(const LinkedList * const linkedList,
                    const LinkedListElement * const item);

/**
 * Initialize a LinkedList.
 *
 * @param linkedList A pointer to the LinkedList to initialize.
 */
void
List_InitLinkedList(LinkedList * const linkedList);

/**
 * Initialize a LinkedListElement.
 *
 * @param item A pointer to the LinkedListElement to initialize.
 */
void
List_InitLinkedListElement(LinkedListElement * const item);

_EXTERN_C_DECL_END

#endif /* LAZULI_LIST_H */

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
typedef struct _Lz_LinkedListElement {
  /** A pointer to the next element in the list */
  struct _Lz_LinkedListElement *next;

  /** A pointer to the previous element in the list */
  struct _Lz_LinkedListElement *prev;
}Lz_LinkedListElement;

/**
 * Represents the main container for doubly linked elements.
 */
typedef struct {
  /** A pointer to the first element of the linked list */
  Lz_LinkedListElement *first;

  /** A pointer to the last element of the linked list */
  Lz_LinkedListElement *last;
}Lz_LinkedList;

/**
 * Define the initialization value for the type Lz_LinkedList.
 */
#define LINKED_LIST_INIT { NULL, NULL }

/**
 * Define the initialization value for the type Lz_LinkedListElement.
 */
#define LINKED_LIST_ELEMENT_INIT { NULL, NULL }

/**
 * Insert an Lz_LinkedListElement as the last element of an existing
 * Lz_LinkedList.
 *
 * @param linkedList A pointer to the linked list head.
 * @param item A pointer to the item to append to the list.
 */
void
List_Append(Lz_LinkedList * const linkedList,
            Lz_LinkedListElement * const item);

/**
 * Insert an Lz_LinkedListElement as the first element of an existing
 * Lz_LinkedList.
 *
 * @param linkedList A pointer to the linked list head.
 * @param item A pointer to the item to prepend to the list.
 */
void
List_Prepend(Lz_LinkedList * const linkedList,
             Lz_LinkedListElement * const item);

/**
 * Move the content of an Lz_LinkedList to the end of an existing Lz_LinkedList.
 *
 * @param linkedListDestination A pointer to the Lz_LinkedList on which to
 *                              append.
 * @param linkedListToMove A pointer to the Lz_LinkedList to move.
 *                         After the operation, this linked list will be empty.
 */
void
List_AppendList(Lz_LinkedList * const linkedListDestination,
                Lz_LinkedList * const linkedListToMove);

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
Lz_LinkedListElement *
List_PickFirst(Lz_LinkedList * const linkedList);

/**
 * Return a pointer to the first element of an existing linked list. This
 * function does not drop the first element of the list.
 *
 * @param linkedList A pointer to an existing Lz_LinkedList.
 *
 * @return A pointer to the first element of the list, or _NULL_ if:
 *         - The @p linkedList is empty
 *         - The parameter @p linkedList is _NULL_
 */
Lz_LinkedListElement *
List_PointFirst(const Lz_LinkedList * const linkedList);

/**
 * Test if an Lz_LinkedList is empty.
 *
 * @param linkedList A pointer to the Lz_LinkedList to test.
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
List_IsEmpty(const Lz_LinkedList * const linkedList);

/**
 * Run through an Lz_LinkedList like a for loop.
 *
 * If the list is empty, no loop is performed, and the execution will continue
 * after the foreach.
 *
 * With configuration option CHECK_NULL_PARAMETERS_IN_LISTS, this implementation
 * can also verify if the @p LINKEDLIST pointer is _NULL_. If so, the loop is
 * not run and the execution continues after the loop.
 *
 * @param LINKEDLIST A pointer to the Lz_LinkedList to run through.
 * @param ITEM A pointer to an Lz_LinkedListElement that will point to the
 *             current item of each loop turn. This pointer will never be
 *             _NULL_.
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
 * Run through an Lz_LinkedList like a for loop.
 *
 * This foreach implementation is typed, so each loop turn will return a typed
 * pointer to the current loop element (ie. not a pointer to a raw
 * Lz_LinkedListElement).
 *
 * If the list is empty, no loop is performed, and the execution will continue
 * after the foreach.
 *
 * With configuration option CHECK_NULL_PARAMETERS_IN_LISTS, this implementation
 * can also verify if the @p LINKEDLIST pointer is _NULL_. If so, the loop is
 * not run and the execution continues after the loop.
 *
 * @param LINKEDLIST A pointer to the Lz_LinkedList to run through.
 * @param TYPE The real type of the list elements
 * @param ITEM A pointer to a struct of type @p TYPE containing the
 *             Lz_LinkedListELement.
 *             This pointer will point to the current item of each loop turn.
 *             This pointer will never be _NULL_ while the loop is running.
 * @param MEMBER The name of the member in @p TYPE which bears the
 *               Lz_LinkedListElement.
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
 * Run through an Lz_LinkedList like a for loop, with the ability of removing
 * elements from the list while iterating on it.
 *
 * An iterator must be provided, in the form of a pointer to an allocated
 * Lz_LinkedListElement. In order to remove elements while iterating, the
 * iterator must be updated with the return value of List_Remove.
 *
 * This foreach implementation is typed, so each loop turn will return a typed
 * pointer to the current loop element (ie. not a pointer to a raw
 * Lz_LinkedListElement).
 *
 * If the list is empty, no loop is performed, and the execution will continue
 * after the foreach.
 *
 * With configuration option CHECK_NULL_PARAMETERS_IN_LISTS, this implementation
 * can also verify if the @p LINKEDLIST pointer is _NULL_. If so, the loop is
 * not run and the execution continues after the loop.
 *
 * @param LINKEDLIST A pointer to the Lz_LinkedList to run through.
 * @param TYPE The real type of the list elements
 * @param ITEM A pointer to a struct of type @p TYPE containing the
 *             Lz_LinkedListELement.
 *             This pointer will point to the current item of each loop turn.
 *             This pointer will never be _NULL_ while the loop is running.
 * @param MEMBER The name of the member in @p TYPE which bears the
 *               Lz_LinkedListElement.
 * @param ITERATOR A pointer to an allocated Lz_LinkedListElement that will be
 *                 used as the loop iterator. This iterator can be updated when
 *                 removing elements from the list, using the return value of
 *                 List_Remove.
 */
#define List_RemovableForEach(LINKEDLIST, TYPE, ITEM, MEMBER, ITERATOR) \
  if ((LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS && (NULL == (LINKEDLIST))) || \
      (NULL == (LINKEDLIST)->first))                                    \
    {}                                                                  \
  else                                                                  \
    for (STATIC_CHECK_TYPE((*(ITERATOR)), Lz_LinkedListElement),        \
           (ITERATOR) = (LINKEDLIST)->first,                            \
           (ITEM) = CONTAINER_OF((LINKEDLIST)->first, MEMBER, TYPE);    \
                                                                        \
         NULL != (ITEM);                                                \
                                                                        \
         ((ITERATOR) = (NULL == (ITERATOR)) ?                           \
          (LINKEDLIST)->first : (ITERATOR)->next),                      \
           ((ITEM) = (NULL == (ITERATOR)) ?                             \
            NULL : CONTAINER_OF((ITERATOR), MEMBER, TYPE)))

/**
 * Insert an element after another in an Lz_LinkedList.
 *
 * @param linkedList A pointer to the LinkedList containing the element
 *                   @p listItem on which to insert after.
 * @param listItem A pointer to an element on which to insert after, already
 *                 present in the @p linkedList.
 * @param itemToInsert A pointer to the item to insert in the list.
 *
 * @warning The @p listItem parameter MUST already be part of the Lz_LinkedList
 *          pointed to by parameter @p linkedList. No check is performed.
 */
void
List_InsertAfter(Lz_LinkedList * const linkedList,
                 Lz_LinkedListElement * const listItem,
                 Lz_LinkedListElement * const itemToInsert);

/**
 * Insert an element before another in a Lz_LinkedList.
 *
 * @param linkedList A pointer to the Lz_LinkedList containing the element
 *                   @p listItem on which to insert before.
 * @param listItem A pointer to an element on which to insert before, already
 *                 present in the @p linkedList.
 * @param itemToInsert A pointer to the item to insert in the list.
 *
 * @warning The @p listItem parameter MUST already be part of the Lz_LinkedList
 *          pointed to by parameter @p linkedList. No check is performed.
 */
void
List_InsertBefore(Lz_LinkedList * const linkedList,
                  Lz_LinkedListElement * const listItem,
                  Lz_LinkedListElement * const itemToInsert);

/**
 * Remove an element from an Lz_LinkedList.
 *
 * @param linkedList A pointer to the Lz_LinkedList containing the element to
 *                   remove.
 * @param itemToRemove A pointer to the element to remove from the list.
 *
 * @return A pointer to the previous element of @p itemToRemove before it is
 *         removed. This return value must be used to update an iterator when
 *         using List_RemovableForEach, in order to allow removing elements from
 *         a list while iterating on it.
 *
 * @warning The @p itemToRemove parameter MUST already be part of the
 *          Lz_LinkedList pointed to by parameter @p linkedList. No check is
 *          performed.
 */
Lz_LinkedListElement *
List_Remove(Lz_LinkedList * const linkedList,
            Lz_LinkedListElement * const itemToRemove);

/**
 * Test if an Lz_LinkedListElement is the last entry of an Lz_LinkedList.
 *
 * @param linkedList A pointer to an Lz_LinkedList.
 * @param item A pointer to the Lz_LinkedListElement to test.
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
List_IsLastElement(const Lz_LinkedList * const linkedList,
                   const Lz_LinkedListElement * const item);

/**
 * Test if an Lz_LinkedListElement is the first entry of an Lz_LinkedList.
 *
 * @param linkedList A pointer to an Lz_LinkedList.
 * @param item A pointer to the Lz_LinkedListElement to test.
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
List_IsFirstElement(const Lz_LinkedList * const linkedList,
                    const Lz_LinkedListElement * const item);


/**
 * Point to an indexed element in an Lz_LinkedList, starting at index 0.
 *
 * The element will not be removed from the list.
 *
 * @warning The complexity of this function is O(n), as it is iterative.
 *
 * @param linkedList A pointer to an Lz_LinkedList.
 * @param index The index of the element to point.
 *
 * @return A pointer to the indexed Lz_LinkedListElement, or _NULL_ if the index
 *         is out of the boundaries of the list.
 */
Lz_LinkedListElement *
List_PointElementAt(const Lz_LinkedList * const linkedList, const size_t index);

/**
 * Initialize an Lz_LinkedList.
 *
 * @param linkedList A pointer to the Lz_LinkedList to initialize.
 */
void
List_InitLinkedList(Lz_LinkedList * const linkedList);

/**
 * Initialize an Lz_LinkedListElement.
 *
 * @param item A pointer to the Lz_LinkedListElement to initialize.
 */
void
List_InitLinkedListElement(Lz_LinkedListElement * const item);

_EXTERN_C_DECL_END

#endif /* LAZULI_LIST_H */

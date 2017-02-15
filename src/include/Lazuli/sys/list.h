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
typedef struct {
  LinkedListElement *next; /** < A pointer to the next element on the list */
}LinkedListElement;

_EXTERN_C_DECL_END

#endif /* LZ_LIST_H */

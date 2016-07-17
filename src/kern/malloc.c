/**
 * @file malloc.c
 *
 * Implementation of dynamic memory allocation.
 * This can serve the kernel or the tasks.
 */

#include <sys/malloc.h>
#include <sys/config.h>
#include <sys/types.h>
#include <sys/arch.h>

/**
 * Set break position of a memory region.
 *
 * @param increment The number of bytes to increment the break.
 * @param map A pointer to the concerned allocation map.
 *
 * @return A pointer to the beginning of the added memory block,
 *         or NULL if can't set break,
 *         or the current break position if increment is zero.
 */
static void *sbrk(unsigned int increment, struct allocation_map *map)
{
  void *stack_pointer;
  unsigned int new_gap;
  unsigned int new_break = (unsigned int)map->brk + increment;

  stack_pointer = get_stack_pointer();
  new_gap = (unsigned int)stack_pointer - new_break;
  
  if (increment == 0) {
	return map->brk;
  }
  
  if (new_gap > BREAK_STACK_GAP) {
	return NULL;
  }

  map->brk += increment;

  return map->brk - increment;
}
#warning comments
/**
 * Get more memory from the heap.
 */
static struct alloc_block *more_core(size_t units,
									 struct allocation_map *map,
									 struct alloc_block *blocks_list_head)
{
  struct alloc_block *new_block;
  
  if (units < MIN_ALLOC_BLOCK) {
	units = MIN_ALLOC_BLOCK;
  }

  new_block = sbrk(units * sizeof(struct alloc_block), map);

  if (new_block == NULL) {
	return NULL;
  }

  new_block->size = units;
  free((void *)(new_block + 1));

  return blocks_list_head; 
}

void *malloc(size_t size,
			 struct allocation_map *map)
{
  unsigned int units;
  struct alloc_block *current, *previous;

  units = (size + sizeof(struct alloc_block) - 1)
	/ sizeof(struct alloc_block) + 1;
  
  previous = map->blocks_list;
  current = previous->next;

  while (current != NULL) {
	if (current->size >= units) {
	  if (current->size == units) {
		previous->next = current->next;
	  } else {
		current->size -= units;
		current += current->size;
		current->size = units;
	  }

	  map->blocks_list = previous;

	  return (void *)(current + 1);
	}

	if (current == map->blocks_list) {
	  if ((current = morecore(units, map)) == NULL) {
		return NULL;
	  }
	}

	previous = current;
	current = current->next;
  }
}

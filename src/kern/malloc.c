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

int
CyclicIntegerDivisionWithModulo(unsigned int a,
								unsigned int b,
								unsigned int *modulo)
{
  unsigned int i = 0;

  if (b == 0) {
	#warning Handle error!
  }
  
  while (a > b) {
	a -= b;
	i++;
  }
  
  *modulo = a;
  return i;
}

int
CyclicIntegerDivision(unsigned int a,
					  unsigned int b)
{
  int modulo;
  
  return CyclicIntegerDivisionWithModulo(a, b, &modulo);
}

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
static void *
Sbrk(unsigned int increment, AllocationMap *map)
{
  ptrdiff_t newGap;
  void *sp;
  void *newBreak = map->brk + increment;

  if (increment == 0) {
	return map->brk;
  }
  
  sp = GetStackPointer();
  newGap = sp - newBreak;
  
  if (newGap < BREAK_STACK_GAP) {
	return NULL;
  }

  map->brk += increment;

  return map->brk - increment;
}

#warning comments
/**
 * Get more memory from the heap.
 */
static AllocationBlock *
MoreCore(size_t s, AllocationMap *map)
{
  AllocationBlock *newBlock;
  
  if (s < MIN_ALLOC_BLOCK) {
	s = MIN_ALLOC_BLOCK;
  }

  newBlock = Sbrk(s * sizeof(*newBlock), map);

  if (newBlock == NULL) {
	return NULL;
  }

  newBlock->size = s;
  BaseFree((void *)(newBlock + 1), map);

  return map->blocksList; 
}

void
BaseFree(void *p, AllocationMap *map)
{
  AllocationBlock *current;
  AllocationBlock *block = (AllocationBlock *)p - 1;

  for (current = map->blocksList;
	   block <= current || block >= current->next;
	   current = current->next) {
	if (current >= current->next &&
		(block > current || block < current->next)) {
	  break;
	}
  }

  if (block + block->size == current->next) {
	block->size += current->next->size;
	block->next = current->next->next;
  } else {
	block->next = current->next;
  }

  if (current + current->size == block) {
	current->size += block->size;
	current->next = block->next;
  } else {
	current->next = block;
  }

  map->blocksList = current;
}

void *
BaseMalloc(size_t s, AllocationMap *map)
{
  AllocationBlock *current, *previous;

  s = CyclicIntegerDivision(s + sizeof(AllocationBlock) - 1,
							sizeof(AllocationBlock) + 1);
  previous = map->blocksList;

  for (current = previous->next;
	   ;
	   previous = current, current = current->next) {
	if (current->size >= s) {
	  if (current->size == s) {
		previous->next = current->next;
	  } else {
		current->size -= s;
		current += current->size;
		current->size = s;
	  }

	  map->blocksList = previous;

	  return (void *)(current + 1);
	}

	if (current == map->blocksList) {
	  if ((current = MoreCore(s, map)) == NULL) {
		return NULL;
	  }
	}
  }
}

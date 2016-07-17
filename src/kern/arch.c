/**
 * @file arch.c
 *
 * Implements architecture specific functions for AVR.
 */

#include <sys/arch.h>
#include <sys/registers.h>

void *get_stack_pointer()
{
  return (void*)SPL;
}

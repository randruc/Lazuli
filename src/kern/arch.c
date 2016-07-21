/**
 * @file arch.c
 *
 * Implements architecture specific functions for AVR.
 */

#include <sys/arch.h>
#include <sys/registers.h>

void *GetStackPointer()
{
  return (void*)SPL;
}

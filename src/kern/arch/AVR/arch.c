/**
 * @file arch.c
 *
 * Implements architecture specific functions for AVR.
 */

#include <sys/arch/AVR/arch.h>
#include <sys/arch/AVR/registers.h>

void *GetStackPointer()
{
  return (void*)SPL;
}

/**
 * @file arch.c
 *
 * Implements architecture specific functions for AVR.
 */

#include <Lazuli/sys/arch/AVR/arch.h>
#include <Lazuli/sys/arch/AVR/registers.h>

void *GetStackPointer()
{
  return (void*)SPL;
}

/* TODO: Check that */
volatile void * const StackPointer = (void*)SPH;

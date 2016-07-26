/**
 * @file kernel.
 *
 * Contains all symbols of the kernel, and routines such as :
 * - Kernel main entry point
 * - System initialization
 */

#include <sys/memory.h>
#include <sys/linker.h>
#include <sys/memory.h>

/**
 * Main entry point for user tasks.
 */
void
Main();

AllocationMap kernelAllocationMap;

/**
 * This is the kernel entry point
 */
void
KMain()
{
  /* Initialize the allocation map for the kernel */
  kernelAllocationMap.baseMem = _brk;
  kernelAllocationMap.brk = _brk;
  kernelAllocationMap.endMem = _ramend;
  kernelAllocationMap.allocationType = ALLOC_UNDEFINED;

  /* Give hand to user */
  Main();
}

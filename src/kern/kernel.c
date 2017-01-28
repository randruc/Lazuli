/**
 * @file kernel.
 *
 * Contains all symbols of the kernel, and routines such as :
 * - Kernel main entry point
 * - System initialization
 * - Kernel panic
 */

#include <sys/memory.h>
#include <sys/linker.h>
#include <sys/memory.h>
#include <sys/arch/AVR/arch.h>
#include <sys/config.h>
#include <sys/types.h>

/**
 * Main entry point for user tasks.
 */
void
Main();

AllocationMap kernelAllocationMap;

/**
 * This is the kernel entry point.
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

#if (ON_PANIC_INFINITE_LOOP + ON_PANIC_SOFTWARE_RESET) != 1
#message "One kernel panic configuration must be defined in config.h."
#endif

void
Panic()
{  
  if (ON_PANIC_INFINITE_LOOP) {
	InfiniteLoop();
  } else if (ON_PANIC_SOFTWARE_RESET) {
  }
}

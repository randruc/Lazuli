/**
 * @file kernel.
 *
 * Contains all symbols of the kernel, and routines such as:
 * - Kernel main entry point
 * - System initialization
 * - Kernel panic
 */

#include <Lazuli/common.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/linker.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/arch/AVR/arch.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/scheduler.h>

/**
 * Main entry point for user tasks.
 */
noreturn void
Main();

extern void
reset_system();

AllocationMap kernelAllocationMap;

/**
 * This is the kernel entry point.
 * This function must never return.
 */
noreturn void
KMain()
{
  /* Initialize the allocation map for the kernel */
  kernelAllocationMap.baseMem = &_brk;
  kernelAllocationMap.brk = &_brk;
  kernelAllocationMap.endMem = &_ramend;
  kernelAllocationMap.allocationType = ALLOC_UNDEFINED;

  Scheduler_Init();

  /* Give hand to user */
  Main();
}

#if (ON_PANIC_INFINITE_LOOP + ON_PANIC_SOFTWARE_RESET) != 1
#message "Only one kernel panic configuration must be defined in config.h."
#endif

void
Panic()
{
  if (ON_PANIC_INFINITE_LOOP) {
    infinite_loop();
  } else if (ON_PANIC_SOFTWARE_RESET) {
    /* TODO: Change this by a watchdog software reset */
    reset_system();
  }
}

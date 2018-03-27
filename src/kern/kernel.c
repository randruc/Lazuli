/**
 * @file src/kern/kernel.c
 * @brief Kernel entry point.
 * @date Jul 2016
 * @author Remi Andruccioli
 *
 * This file describes all the kernel routines such as:
 * - Kernel main entry point after startup, system initialization
 * - Kernel panic
 *
 * It also holds the kernel memory allocation map.
 */

#include <Lazuli/common.h>

#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/linker.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/scheduler_base.h>
#include <Lazuli/sys/arch/arch.h>

/**
 * Main entry point for user tasks.
 *
 * This is the function the user has to define to register his own tasks.
 */
int
main();

/**
 * The allocation map for the whole kernel.
 */
AllocationMap kernelAllocationMap;

/**
 * This is the kernel entry point.
 * This function must never return.
 */
void
KMain()
{
  /* Initialize the allocation map for the kernel */
  kernelAllocationMap.baseMem = &_brk;
  kernelAllocationMap.brk = &_brk;
  kernelAllocationMap.endMem = &_ramend;
  kernelAllocationMap.allocationType = ALLOC_UNDEFINED;

  BaseSchedulerInit();

  /* Give hand to user */
  main();
}

#if (ON_PANIC_INFINITE_LOOP + ON_PANIC_SOFTWARE_RESET) != 1
#error "Only one kernel panic configuration must be defined in config.h."
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

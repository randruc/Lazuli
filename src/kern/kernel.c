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

#include <stdint.h>

#include <Lazuli/common.h>

#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/config.h>
#include <Lazuli/sys/linker.h>
#include <Lazuli/sys/memory.h>

#include <Lazuli/sys/scheduler_base.h>

/**
 * Main entry point for user tasks.
 *
 * This is the function the user has to define to register his own tasks.
 */
void
main(void);

/**
 * The allocation map for the whole kernel.
 */
AllocationMap kernelAllocationMap;

/**
 * This is the kernel entry point.
 * This function must never return.
 */
void
KMain(void)
{
  /* Initialize the allocation map for the kernel */
  kernelAllocationMap.baseMem = &_brk;
  kernelAllocationMap.brk = &_brk;
  kernelAllocationMap.endMem = &_ramend;
  kernelAllocationMap.allocationType = ALLOC_UNDEFINED;

  Arch_InitIdleCpuMode();

  if (LZ_CONFIG_USE_SERIAL) {
    Arch_InitSerial();
  }

  /* Give hand to user */
  main();
}

/* TODO: Find something better to perform this verification... */
#if (LZ_CONFIG_ON_PANIC_INFINITE_LOOP + LZ_CONFIG_ON_PANIC_SOFTWARE_RESET) != 1
#error "Only one kernel panic configuration must be defined in config.h."
#endif

NORETURN void
Panic(void)
{
  if (LZ_CONFIG_ON_PANIC_INFINITE_LOOP) {
    Arch_InfiniteLoop();
  } else if (LZ_CONFIG_ON_PANIC_SOFTWARE_RESET) {
    /* TODO: Change this by a watchdog software reset */
    Arch_ResetSystem();
  }
}

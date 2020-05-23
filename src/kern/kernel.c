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
#include <Lazuli/config.h>

#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/linker.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler.h>

/**
 * Main entry point for user tasks.
 *
 * This is the function the user has to define to register his own tasks.
 */
int
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
Kernel_Main(void)
{
  /* Initialize the allocation map for the kernel */
  kernelAllocationMap.baseMem = &_brk;
  kernelAllocationMap.brk = &_brk;
  kernelAllocationMap.endMem = &_ramend;
  kernelAllocationMap.allocationType = ALLOC_UNDEFINED;

  Arch_InitIdleCpuMode();
  Scheduler_Init();

  if (LZ_CONFIG_MODULE_SERIAL_USED) {
    Arch_InitSerial();
  }

  if (LZ_CONFIG_INSTRUMENT_CONTEXT_SWITCHES) {
    Arch_InitInstrumentation();
  }

  /* Give hand to user */
  main();
}

/* TODO: Find something better to perform this verification... */
#ifndef LZ_STATIC_ANALYSIS
#if (LZ_CONFIG_ON_PANIC_INFINITE_LOOP + LZ_CONFIG_ON_PANIC_SOFTWARE_RESET) != 1
#error "Only one kernel panic configuration must be defined in config.h."
#endif
#endif

NORETURN void
Kernel_Panic(void)
{
  if (LZ_CONFIG_ON_PANIC_INFINITE_LOOP) {
    Arch_InfiniteLoop();
  } else if (LZ_CONFIG_ON_PANIC_SOFTWARE_RESET) {
    /* TODO: Change this by a watchdog software reset */
    Arch_ResetSystem();
  }
}

/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Kernel entry point.
 * @copyright 2016-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file describes all the kernel routines such as:
 * - Kernel main entry point after startup, system initialization
 * - Kernel panic
 *
 * It also holds the kernel memory allocation map.
 */

#include <stdint.h>

#include <Lazuli/common.h>
#include <Lazuli/config.h>

#include <Lazuli/sys/arch/arch.h>
#include <Lazuli/sys/compiler.h>
#include <Lazuli/sys/kernel.h>
#include <Lazuli/sys/linker.h>
#include <Lazuli/sys/memory.h>
#include <Lazuli/sys/scheduler.h>

/**
 * Main entry point for user tasks.
 *
 * This is the function the user has to define to register his own tasks.
 *
 * This prototype differs from the standard "int main(void)" as the return value
 * is useless here.
 */
void
main(void);

/*
 * Initialized at system startup. Can be usable only after the .data section
 * have been initialized. So don't access it before.
 */
volatile lz_system_status_t systemStatus = SYSTEM_STATUS_FLAG_IN_KERNEL;

NOINIT AllocationMap kernelAllocationMap;

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

void
Kernel_Panic(void)
{
  if (LZ_CONFIG_ON_PANIC_INFINITE_LOOP) {
    Arch_InfiniteLoop();
  } else if (LZ_CONFIG_ON_PANIC_SOFTWARE_RESET) {
    /* TODO: Change this by a watchdog software reset */
    Arch_ResetSystem();
  }
}

void
Kernel_ManageFailure(void)
{
  if (SYSTEM_STATUS_IS_IN_KERNEL) {
    /* We are running in the kernel */
    Kernel_Panic();
  } else {
    /* We are running in a task */
    Scheduler_AbortTask();
  }
}

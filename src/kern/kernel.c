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

  Arch_Init();

  /* Give hand to user */
  main();
}

/* TODO: Find something better to perform this verification... */
#if (CONFIG_ON_PANIC_INFINITE_LOOP + CONFIG_ON_PANIC_SOFTWARE_RESET) != 1
#error "Only one kernel panic configuration must be defined in config.h."
#endif

void
Panic()
{
  if (CONFIG_ON_PANIC_INFINITE_LOOP) {
    Arch_InfiniteLoop();
  } else if (CONFIG_ON_PANIC_SOFTWARE_RESET) {
    /* TODO: Change this by a watchdog software reset */
    Arch_ResetSystem();
  }
}

/* TODO: Move the ReverseByte utilities somewhere else */
/**
 * Provides a way to manipulate a 16-bit word as an array of bytes.
 */
union WordAsBytesArray
{
  uint16_t u16Value;    /**< The 16-bit word to manipulate as a bytes array. */
  uint8_t  u8Values[2]; /**< The bytes array to manipulate the 16-bit word.  */
};

void
ReverseBytes16(void * const value)
{
  union WordAsBytesArray reverse;

  reverse.u16Value = *(uint16_t*)value;
  ((uint8_t *)value)[0] = reverse.u8Values[1];
  ((uint8_t *)value)[1] = reverse.u8Values[0];
}

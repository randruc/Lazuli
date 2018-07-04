/**
 * @file src/include/Lazuli/sys/config.h
 * @brief Kernel configuration.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file contains all the configuration for the kernel.
 * These are only macro constants.
 * Does not contain any arch-specific configuration.
 */

#ifndef LZ_SYS_CONFIG_H
#define LZ_SYS_CONFIG_H

/**
 * Size in bytes of the security gap between the break and the stack pointer.
 */
#define BREAK_STACK_GAP (50)

/**
 * Clock frequency in Hertz.
 */
#define CLOCK_FREQUENCY (1000000)

/**
 * When 1, run an infinite loop on kernel panic.
 *
 * When 0, doesn't apply.
 */
#define ON_PANIC_INFINITE_LOOP (1)

/**
 * When 1, perform a software reset on kernel panic.
 *
 * When 0, doesn't apply.
 */
#define ON_PANIC_SOFTWARE_RESET (0)

/**
 * Default stack size in bytes for a new task.
 */
#define DEFAULT_TASK_STACK_SIZE (20)

/**
 * When 1, always check for NULL function parameters in linked lists
 * implementation.
 *
 * When 0, never check for NULL parameters.
 * This is a way to obtain better performances, but it's also less safe.
 */
#define CHECK_NULL_PARAMETERS_IN_LISTS (1)

/**
 * Use Round-Robin scheduler.
 */
#define USE_SCHEDULER_RR (1)

/**
 * Use Highest Priority First scheduler.
 */
#define USE_SCHEDULER_HPF (1)

/**
 * Check if the interrupt code sent by the ASM interrupt handler to the base
 * scheduler interrupt handler is an admissible value.
 */
#define CHECK_INTERRUPT_CODE_OVER_LAST_ENTRY (1)

/**
 * Configure if the idle task must have a name.
 *
 * If set to 1, the idle task will have the name "idle" attached to it, and 5
 * bytes will be used to store the name in read-only data section (4 bytes for
 * the string + 1 byte for the string termination NUL character).
 *
 * If set to 0, the idle task will have no name attached to it. Thus it can save
 * 5 bytes in the read-only data section.
 */
#define IDLE_TASK_HAS_NAME (1)

/**
 * If set to 1, put the CPU to sleep when it's idle.
 * If set to 0, the CPU will actively infinite loop when it's idle.
 */
#define ON_IDLE_SLEEP (0)

/**
 * Enter "Idle Mode" when sleeping.
 *
 * This is the default value no sleep mode is selected.
 */
#define SLEEP_WITH_IDLE_MODE (1)

/**
 * Enter "ADC Noise Reduction Mode" when sleeping.
 */
#define SLEEP_WITH_ADC_NOISE_REDUCTION_MODE (0)

/**
 * Enter "Power-down Mode" when sleeping.
 */
#define SLEEP_WITH_POWER_DOWN_MODE (0)

/**
 * Enter "Power-save Mode" when sleeping.
 */
#define SLEEP_WITH_POWER_SAVE_MODE (0)

/**
 * Enter "Standby Mode" when sleeping.
 */
#define SLEEP_WITH_STANDBY_MODE (0)

/**
 * Enter "Extended Standby Mode" when sleeping.
 */
#define SLEEP_WITH_EXTENDED_STANDBY_MODE (0)

#endif /* LZ_SYS_CONFIG_H */

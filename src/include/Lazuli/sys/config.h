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

#ifndef LAZULI_SYS_CONFIG_H
#define LAZULI_SYS_CONFIG_H

/**
 * Size in bytes of the security gap between the break and the stack pointer.
 */
#define CONFIG_BREAK_STACK_GAP (50)

/**
 * Clock frequency in Hertz.
 */
#define CONFIG_CLOCK_FREQUENCY (1000000)

/**
 * When 1, run an infinite loop on kernel panic.
 *
 * When 0, doesn't apply.
 */
#define CONFIG_ON_PANIC_INFINITE_LOOP (1)

/**
 * When 1, perform a software reset on kernel panic.
 *
 * When 0, doesn't apply.
 */
#define CONFIG_ON_PANIC_SOFTWARE_RESET (0)

/**
 * Default stack size in bytes for a new task.
 */
#define CONFIG_DEFAULT_TASK_STACK_SIZE (20)

/**
 * Default priority for a new task.
 */
#define CONFIG_DEFAULT_TASK_PRIORITY (0)

/**
 * When 1, always check for NULL functions parameters in linked lists
 * implementation.
 *
 * When 0, never check for NULL parameters.
 *
 * This is a way to obtain better performances, but it's also less safe.
 */
#define CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS (1)

/**
 * Use Round-Robin scheduler.
 */
#define CONFIG_USE_SCHEDULER_RR (1)

/**
 * Use Highest Priority First scheduler.
 */
#define CONFIG_USE_SCHEDULER_HPF (1)

/** @name HPF Scheduling */
/** @{                   */

/**
 * The stack size in bytes of the HPF scheduler idle task.
 */
#define CONFIG_HPF_IDLE_TASK_STACK_SIZE (10)

/**
 * The priority of the HPF scheduler idle task.
 */
#define CONFIG_HPF_IDLE_TASK_PRIORITY (-1)

/**
 * Configure if the idle task must have a name.
 *
 * If set to 1, the idle task will have a name attached to it, and the necessary
 * bytes will be used to store this name in read-only data section. This name is
 * defined in this configuration file.
 *
 * If set to 0, the idle task will have no name attached to it. Thus it can save
 * a few bytes in the read-only data section.
 */
#define CONFIG_HPF_IDLE_TASK_HAS_NAME (1)

/**
 * The name of the HPF idle task.
 * Never used if HPF_IDLE_TASK_HAS_NAME is set to 0.
 */
#define CONFIG_HPF_IDLE_TASK_NAME "hpf-idle"

/** @} */

/**
 * Check if the interrupt code sent by the ASM interrupt handler to the base
 * scheduler interrupt handler is an admissible value.
 */
#define CONFIG_CHECK_INTERRUPT_CODE_OVER_LAST_ENTRY (1)

/**
 * If set to 1, put the CPU to sleep when it's idle.
 * If set to 0, the CPU will actively infinite loop when it's idle.
 */
#define CONFIG_ON_IDLE_SLEEP (0)

/** @name AVR-specific configuration */
/** @{                               */

/**
 * Enter "Idle Mode" when sleeping.
 *
 * This is the default value no sleep mode is selected.
 */
#define CONFIG_SLEEP_WITH_IDLE_MODE (1)

/**
 * Enter "ADC Noise Reduction Mode" when sleeping.
 */
#define CONFIG_SLEEP_WITH_ADC_NOISE_REDUCTION_MODE (0)

/**
 * Enter "Power-down Mode" when sleeping.
 */
#define CONFIG_SLEEP_WITH_POWER_DOWN_MODE (0)

/**
 * Enter "Power-save Mode" when sleeping.
 */
#define CONFIG_SLEEP_WITH_POWER_SAVE_MODE (0)

/**
 * Enter "Standby Mode" when sleeping.
 */
#define CONFIG_SLEEP_WITH_STANDBY_MODE (0)

/**
 * Enter "Extended Standby Mode" when sleeping.
 */
#define CONFIG_SLEEP_WITH_EXTENDED_STANDBY_MODE (0)

/** @} */

#endif /* LAZULI_SYS_CONFIG_H */

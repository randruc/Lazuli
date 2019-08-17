/**
 * @file src/include/Lazuli/sys/config.h
 * @brief Kernel configuration.
 * @date Feb 2017
 * @author Remi Andruccioli
 *
 * This file contains all the configuration for the kernel.
 * These are only macro constants. This file must be kept simple so it can be
 * included in ASM files.
 */

#ifndef LAZULI_SYS_CONFIG_H
#define LAZULI_SYS_CONFIG_H

/**
 * Use Rate Monotonic Scheduler.
 */
#define LZ_CONFIG_USE_SCHEDULER_RMS (1)

/**
 * Size in bytes of the security gap between the break and the stack pointer.
 */
#define LZ_CONFIG_BREAK_STACK_GAP (50)

/**
 * THe clock frequency of the machine, in Hertz.
 */
#define LZ_CONFIG_MACHINE_CLOCK_FREQUENCY (16000000)

/**
 * The operating system clock resolution frequency, in Hertz.
 */
#define LZ_CONFIG_SYSTEM_CLOCK_RESOLUTION_FREQUENCY (50)

/**
 * When 1, run an infinite loop on kernel panic.
 *
 * When 0, doesn't apply.
 */
#define LZ_CONFIG_ON_PANIC_INFINITE_LOOP (1)

/**
 * When 1, perform a software reset on kernel panic.
 *
 * When 0, doesn't apply.
 */
#define LZ_CONFIG_ON_PANIC_SOFTWARE_RESET (0)

/**
 * Default stack size in bytes for a new task.
 */
#define LZ_CONFIG_DEFAULT_TASK_STACK_SIZE (20)

/**
 * Default priority for a new task.
 */
#define LZ_CONFIG_DEFAULT_TASK_PRIORITY (0)

/**
 * When 1, always check for NULL functions parameters in linked lists
 * implementation.
 *
 * When 0, never check for NULL parameters.
 *
 * This is a way to obtain better performances, but it's also less safe.
 */
#define LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS (1)

/**
 * When 1, save the task's context when it terminates (i.e. when the task
 * returns from its main function.
 *
 * When 0, don't save the task's context.
 * Setting to 0 will result in slightly better performances when a task
 * terminates.
 */
#define LZ_CONFIG_SAVE_TASK_CONTEXT_ON_TERMINATION (0)

/**
 * Check if the interrupt code sent by the ASM interrupt handler to the base
 * scheduler interrupt handler is an admissible value.
 */
#define LZ_CONFIG_CHECK_INTERRUPT_CODE_OVER_LAST_ENTRY (1)

/* TODO: Check if this one is used */
/**
 * If set to 1, put the CPU to sleep when it's idle.
 * If set to 0, the CPU will actively infinite loop when it's idle.
 */
#define LZ_CONFIG_ON_IDLE_SLEEP (0)

/** @name RMS Scheduling */
/** @{                   */
/**
 * The stack size in bytes of the RMS scheduler idle task.
 */
#define LZ_CONFIG_RMS_IDLE_TASK_STACK_SIZE (10)

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
#define LZ_CONFIG_RMS_IDLE_TASK_HAS_NAME (1)

/**
 * The name of the RMS idle task.
 * Never used if RMS_IDLE_TASK_HAS_NAME is set to 0.
 */
#define LZ_CONFIG_RMS_IDLE_TASK_NAME "rms-idle-task"
/** @} */

/** @name Spinlocks */
/** @{              */

/**
 * When 1, always check for NULL functions parameters in spinlocks
 * implementation.
 *
 * When 0, never check for NULL parameters.
 *
 * This is a way to obtain better performances, but it's also less safe.
 */
#define LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_SPINLOCKS (1)

/** @}              */

/** @name Mutexes */
/** @{            */

/**
 * When 1, always check for NULL functions parameters in mutexes
 * implementation.
 *
 * When 0, never check for NULL parameters.
 *
 * This is a way to obtain better performances, but it's also less safe.
 */
#define LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES (1)

/** @}            */

/** @name Serial */
/** @{           */

/**
 * When 1, always check for NULL functions parameters in serial API.
 *
 * When 0, never check for NULL parameters.
 *
 * This is one way to obtain better performances, but it's also less safe.
 */
#define LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_SERIAL (1)

/**
 * When 1, check for enum parameters that are over the admissible values defined
 * by the enum.
 *
 * When 0, never check the value of enum parameters.
 *
 * This is one way to obtain better performances, but it's also less safe.
 */
#define LZ_CONFIG_CHECK_WRONG_ENUM_ENTRIES_IN_SERIAL (1)

/**
 * When 1, serial input and output will use interrupt blocking.
 *
 * When 0, serial input and output will use active waiting (spin).
 */
#define LZ_CONFIG_SERIAL_USE_INTERRUPTS (0)

/** @}           */

/** @name AVR-specific configuration */
/** @{                               */

/**
 * Enter "Idle Mode" when sleeping.
 *
 * This is the default value no sleep mode is selected.
 */
#define LZ_CONFIG_SLEEP_WITH_IDLE_MODE (1)

/**
 * Enter "ADC Noise Reduction Mode" when sleeping.
 */
#define LZ_CONFIG_SLEEP_WITH_ADC_NOISE_REDUCTION_MODE (0)

/**
 * Enter "Power-down Mode" when sleeping.
 */
#define LZ_CONFIG_SLEEP_WITH_POWER_DOWN_MODE (0)

/**
 * Enter "Power-save Mode" when sleeping.
 */
#define LZ_CONFIG_SLEEP_WITH_POWER_SAVE_MODE (0)

/**
 * Enter "Standby Mode" when sleeping.
 */
#define LZ_CONFIG_SLEEP_WITH_STANDBY_MODE (0)

/**
 * Enter "Extended Standby Mode" when sleeping.
 */
#define LZ_CONFIG_SLEEP_WITH_EXTENDED_STANDBY_MODE (0)

/** @} */

#endif /* LAZULI_SYS_CONFIG_H */

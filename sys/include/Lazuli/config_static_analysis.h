/*
 * SPDX-License-Identifier: GPL-3.0-only
 * This file is part of Lazuli.
 */

/**
 * @file
 * @brief Lazuli configuration file for static analysis.
 * @copyright 2020, Remi Andruccioli <remi.andruccioli@gmail.com>
 *
 * This file is a redefinition of all the configuration macro constants that are
 * defined in config.h.in. All the macro constants are defined here as if they
 * were true C extern symbols.
 * The reason for that is that most static code analysis tools operate on
 * preprocessed files. By using this file instead of the final config.h, we
 * simulate the fact that these configuration constants can take any value.
 * So static analysizers don't complain/exclude/assume some parts of source
 * code.
 *
 * @warning Some of these definitions can cause problems as there are some
 *          places in the code where a true constant is expected. e.g when
 *          initializing fields of a constant struct. In these case, static
 *          analyzers can complain.
 */

#ifndef LAZULI_CONFIG_STATIC_ANALYSIS_H
#define LAZULI_CONFIG_STATIC_ANALYSIS_H

#include <Lazuli/common.h>
#include <Lazuli/lazuli.h>

/**
 * The Lazuli kernel version as a string, defined as MAJOR.MINOR.PATCH.
 */
extern const char * const LZ_VERSION;

/**
 * The Lazuli kernel MAJOR version number.
 */
extern const int LZ_VERSION_MAJOR;

/**
 * The Lazuli kernel MINOR version number.
 */
extern const int LZ_VERSION_MINOR;

/**
 * The Lazuli kernel PATCH version number.
 */
extern const int LZ_VERSION_PATCH;

/**
 * Size in bytes of the security gap between the break and the stack pointer.
 */
extern const size_t LZ_CONFIG_BREAK_STACK_GAP;

/**
 * The clock frequency of the machine, in Hertz.
 */
extern const unsigned int LZ_CONFIG_MACHINE_CLOCK_FREQUENCY;

/**
 * The operating system clock resolution frequency, in Hertz.
 */
extern const unsigned int LZ_CONFIG_SYSTEM_CLOCK_RESOLUTION_FREQUENCY;

/**
 * When 1, run an infinite loop on kernel panic.
 *
 * When 0, doesn't apply.
 */
extern const bool LZ_CONFIG_ON_PANIC_INFINITE_LOOP;

/**
 * When 1, perform a software reset on kernel panic.
 *
 * When 0, doesn't apply.
 */
extern const bool LZ_CONFIG_ON_PANIC_SOFTWARE_RESET;

/**
 * Default stack size in bytes for a new task.
 */
extern const size_t LZ_CONFIG_DEFAULT_TASK_STACK_SIZE;

/**
 * Default priority for a new task.
 */
extern const lz_task_priority_t LZ_CONFIG_DEFAULT_TASK_PRIORITY;

/**
 * When 1, always check for NULL functions parameters in linked lists
 * implementation.
 *
 * When 0, never check for NULL parameters.
 *
 * This is a way to obtain better performances, but it's also less safe.
 */
extern const bool LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS;

/**
 * When 1, check if the interrupt code sent by the ASM interrupt handler to the
 * base scheduler interrupt handler is an admissible value.
 *
 * When 0, don't check.
 */
extern const bool LZ_CONFIG_CHECK_INTERRUPT_CODE_OVER_LAST_ENTRY;

/**
 * When 1, put the CPU to sleep when it's idle.
 * When 0, the CPU will actively infinite loop when it's idle.
 */
extern const bool LZ_CONFIG_ON_IDLE_SLEEP;

/**
 * The sequence to use for new lines on the serial line.
 */
#define LZ_CONFIG_SERIAL_NEWLINE "\r\n"

/** @name Scheduling */
/** @{               */
/**
 * The stack size in bytes of the scheduler idle task.
 */
extern const size_t LZ_CONFIG_IDLE_TASK_STACK_SIZE;

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
extern const bool LZ_CONFIG_IDLE_TASK_HAS_NAME;

/**
 * The name of the idle task.
 * Never used if LZ_CONFIG_IDLE_TASK_HAS_NAME is set to 0.
 */
extern const char * const LZ_CONFIG_IDLE_TASK_NAME;

/**
 * When set, add instrumentation code to measure context switches.
 */
extern const bool LZ_CONFIG_INSTRUMENT_CONTEXT_SWITCHES;

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
extern const bool LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_SPINLOCKS;

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
extern const bool LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES;

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
extern const bool LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_SERIAL;

/**
 * When 1, check for enum parameters that are over the admissible values defined
 * by the enum.
 *
 * When 0, never check the value of enum parameters.
 *
 * This is one way to obtain better performances, but it's also less safe.
 */
extern const bool LZ_CONFIG_CHECK_WRONG_ENUM_ENTRIES_IN_SERIAL;

/**
 * When 1, serial input and output will use interrupt blocking.
 *
 * When 0, serial input and output will use active waiting (spin).
 */
extern const bool LZ_CONFIG_SERIAL_USE_INTERRUPTS;

/** @}           */

/** @name AVR-specific configuration */
/** @{                               */

/**
 * Enter "Idle Mode" when sleeping.
 *
 * This is the default value no sleep mode is selected.
 */
extern const bool LZ_CONFIG_AVR_SLEEP_WITH_IDLE_MODE;

/**
 * Enter "ADC Noise Reduction Mode" when sleeping.
 */
extern const bool LZ_CONFIG_AVR_SLEEP_WITH_ADC_NOISE_REDUCTION_MODE;

/**
 * Enter "Power-down Mode" when sleeping.
 */
extern const bool LZ_CONFIG_AVR_SLEEP_WITH_POWER_DOWN_MODE;

/**
 * Enter "Power-save Mode" when sleeping.
 */
extern const bool LZ_CONFIG_AVR_SLEEP_WITH_POWER_SAVE_MODE;

/**
 * Enter "Standby Mode" when sleeping.
 */
extern const bool LZ_CONFIG_AVR_SLEEP_WITH_STANDBY_MODE;

/**
 * Enter "Extended Standby Mode" when sleeping.
 */
extern const bool LZ_CONFIG_AVR_SLEEP_WITH_EXTENDED_STANDBY_MODE;

/** @} */

/** @name Used modules */
/** @{                 */

/*
 * Declared in the same order as CMakeLists.txt.
 */

/**
 * Use module "clock_24": Implement a 24-Hour clock in the kernel.
 */
extern const bool LZ_CONFIG_MODULE_CLOCK_24_USED;

/**
 * Use module "mutex": Mutexes implementation.
 */
extern const bool LZ_CONFIG_MODULE_MUTEX_USED;

/**
 * Use module "serial": Serial interface configuration.
 */
extern const bool LZ_CONFIG_MODULE_SERIAL_USED;

/**
 * Use module "spinlock": Spinlocks implementation.
 */
extern const bool LZ_CONFIG_MODULE_SPINLOCK_USED;

/**
 * The port used for instrumentation on AVR machines.
 */
extern uint16_t LZ_CONFIG_AVR_INSTRUMENT_PORT;

/**
 * The position in the port used for instrumentation on AVR machines.
 */
extern uint8_t LZ_CONFIG_AVR_INSTRUMENT_POSITION;

/** @} */

#endif /* LAZULI_CONFIG_STATIC_ANALYSIS_H */

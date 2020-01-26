#
# Initial cache settings used when generating the build system.
# Contains all configuration variables and options.
#
# Every cache variable in this file must contain a 1-line string describing its
# function.
#


## General Settings

set(
  LZ_CONFIG_BREAK_STACK_GAP
  50
  CACHE STRING
  "Size in bytes of the security gap between the break and the stack pointer.")

set(
  LZ_CONFIG_MACHINE_CLOCK_FREQUENCY
  16000000
  CACHE STRING
  "The clock frequency of the machine, in Hertz.")

set(
  LZ_CONFIG_SYSTEM_CLOCK_RESOLUTION_FREQUENCY
  50
  CACHE STRING
  "The operating system clock resolution frequency, in Hertz.")

option(
  LZ_CONFIG_ON_PANIC_INFINITE_LOOP
  "When 1, perform a software reset on kernel panic. When 0, doesn't apply."
  ON)

option(
  LZ_CONFIG_ON_PANIC_SOFTWARE_RESET
  "When 1, perform a software reset on kernel panic. When 0, doesn't apply."
  OFF)

set(
  LZ_CONFIG_DEFAULT_TASK_STACK_SIZE
  20
  CACHE STRING
  "Default stack size in bytes for a new task.")

set(
  LZ_CONFIG_DEFAULT_TASK_PRIORITY
  0
  CACHE STRING
  "Default priority for a new task.")

option(
  LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_LISTS
  "Check for NULL functions parameters in linked lists implementation."
  ON)

option(
  LZ_CONFIG_SAVE_TASK_CONTEXT_ON_TERMINATION
  "Save the task's context when it terminates."
  ON)

option(
  LZ_CONFIG_CHECK_INTERRUPT_CODE_OVER_LAST_ENTRY
  "Check the interrupt code sent by the ASM interrupt handler."
  ON)

option(
  LZ_CONFIG_ON_IDLE_SLEEP
  "Put the CPU to sleep when it's idle, or infinite loop."
  ON)


## Scheduling

set(
  LZ_CONFIG_IDLE_TASK_STACK_SIZE
  10
  CACHE STRING
  "The stack size in bytes of the scheduler idle task.")

option(
  LZ_CONFIG_IDLE_TASK_HAS_NAME
  "Configure if the idle task must have a name."
  ON)

set(
  LZ_CONFIG_IDLE_TASK_NAME
  "idle-task"
  CACHE STRING
  "The name of the idle task.")


## Spinlocks

option(
  LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_SPINLOCKS
  "Check for NULL functions parameters in spinlocks."
  ON)


## Mutexes

option(
  LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_MUTEXES
  "Check for NULL functions parameters in mutexes."
  ON)


## Serial

option(
  LZ_CONFIG_CHECK_NULL_PARAMETERS_IN_SERIAL
  "Check for NULL functions parameters in serial API."
  ON)

option(
  LZ_CONFIG_CHECK_WRONG_ENUM_ENTRIES_IN_SERIAL
  "Check for enum parameters that are over the admissible values."
  ON)

option(
  LZ_CONFIG_SERIAL_USE_INTERRUPTS
  "Serial input and output will use interrupt blocking, or active waiting."
  OFF)


## AVR-specific

option(
  LZ_CONFIG_SLEEP_WITH_IDLE_MODE
  "Enter 'Idle Mode' when sleeping."
  ON)

option(
  LZ_CONFIG_SLEEP_WITH_ADC_NOISE_REDUCTION_MODE
  "Enter 'ADC Noise Reduction Mode' when sleeping."
  OFF)

option(
  LZ_CONFIG_SLEEP_WITH_POWER_DOWN_MODE
  "Enter 'Power-down Mode' when sleeping."
  OFF)

option(
  LZ_CONFIG_SLEEP_WITH_POWER_SAVE_MODE
  "Enter 'Power-save Mode' when sleeping."
  OFF)

option(
  LZ_CONFIG_SLEEP_WITH_STANDBY_MODE
  "Enter 'Standby Mode' when sleeping."
  OFF)

option(
  LZ_CONFIG_SLEEP_WITH_EXTENDED_STANDBY_MODE
  "Enter 'Extended Standby Mode' when sleeping."
  OFF)
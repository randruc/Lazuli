#
# Main CMake file to build the Lazuli kernel.
#

cmake_minimum_required(VERSION 3.11)

set(CMAKE_VERBOSE_MAKEFILE OFF)

include(cmake/machine_choice.cmake)
include(cmake/initial_cache.cmake)

if(NOT LZ_TARGET_MACHINE_CHOICE)
  message(
    FATAL_ERROR
    "Configuration error: \
    The target machine must be defined with cache variable \
    'LZ_TARGET_MACHINE_CHOICE'.")
elseif(LZ_TARGET_MACHINE_CHOICE STREQUAL AVR_ATmega328p)
  set(
    CMAKE_TOOLCHAIN_FILE
    cmake/avr.toolchain.cmake)
else()
  message(
    FATAL_ERROR
    "Fatal error: \
    The target machine choice '${LZ_TARGET_MACHINE_CHOICE}' is not a known \
    value.")
endif()

set(
  ALL_FLAGS
  -DLZ_DEBUG=1
  -g
  -ansi
  -std=c89
  -pedantic
  -Wall
  -Wextra
  -Werror
  -mmcu=atmega328p
  -O2
  -nostartfiles
  -nostdlib
  -nodefaultlibs
  -ffreestanding
  -fshort-enums
  -DLZ_CONFIG_USE_MUTEX=1
  -DLZ_CONFIG_USE_SPINLOCK=1
  -DLZ_CONFIG_USE_SERIAL=1
  -DLZ_CONFIG_USE_CLOCK_24=1)

set(
  LAZULI_CORE_SOURCE_FILES
  kern/arch/AVR/arch.c
  kern/arch/AVR/interrupt_vectors_table.S
  kern/arch/AVR/startup.S
  kern/arch/AVR/spinlock.S
  kern/spinlock.c
  kern/arch/AVR/timer_counter_1.c
  kern/arch/AVR/usart.c  
  kern/kernel.c
  kern/memory.c
  kern/scheduler.c
  kern/list.c
  kern/mutex.c
  kern/arch/AVR/mutex.S
  kern/serial.c
  kern/clock_24.c)

project(
  Lazuli
  LANGUAGES C ASM
  VERSION 0.1.0)

message("Target machine is: ${LZ_TARGET_MACHINE_CHOICE}")

configure_file(
  config.h.in
  config.h
  @ONLY
  NEWLINE_STYLE UNIX)

include_directories(
  BEFORE
  include                         # For Lazuli headers
  libc-headers/arch-dependent/AVR # For libc headers
  ${PROJECT_BINARY_DIR})          # For auto-generated config.h

add_library(
  Lazuli
  STATIC
  ${LAZULI_CORE_SOURCE_FILES})

set_target_properties(
  Lazuli
  PROPERTIES
  COMPILE_FLAGS ${ALL_FLAGS}
  LINK_FLAGS ${ALL_FLAGS})

target_compile_options(
  Lazuli
  PUBLIC ${ALL_FLAGS})

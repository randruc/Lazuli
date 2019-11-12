#
# Main CMake file to build the Lazuli kernel as a static library.
#

cmake_minimum_required(VERSION 3.11)

set(CMAKE_VERBOSE_MAKEFILE OFF)

include(cmake/machine_choice.cmake)
include(cmake/initial_cache.cmake)

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

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  set(
    LZ_KERNEL_COMPILE_FLAGS
    -DLZ_DEBUG=1
    -g
    -Wall
    -Wextra
    -Werror
    -O2
    -ansi
    -std=c89
    -pedantic
    -fshort-enums
    -mmcu=atmega328p
    -DLZ_CONFIG_USE_MUTEX=1
    -DLZ_CONFIG_USE_SPINLOCK=1
    -DLZ_CONFIG_USE_SERIAL=1
    -DLZ_CONFIG_USE_CLOCK_24=1)
else()
  message(
    FATAL_ERROR
    "Fatal error: The compiler of ID ${CMAKE_C_COMPILER_ID} is unknown.")
endif()

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

set(
  KERNEL_LIBRARY_NAME
  ${CMAKE_PROJECT_NAME})#_${LZ_TARGET_MACHINE_CHOICE}_${CMAKE_PROJECT_VERSION})

set(
  KERNEL_LST_NAME
  ${KERNEL_LIBRARY_NAME}.lst)

add_library(
  ${KERNEL_LIBRARY_NAME}
  STATIC
  ${LAZULI_CORE_SOURCE_FILES})

target_compile_options(
  ${KERNEL_LIBRARY_NAME}
  PUBLIC ${LZ_KERNEL_COMPILE_FLAGS})

add_custom_command(
  OUTPUT ${KERNEL_LST_NAME}
  COMMAND ${CMAKE_OBJDUMP} -hS libLazuli.a > ${KERNEL_LST_NAME})

add_custom_target(
  ${KERNEL_LIBRARY_NAME}
  ALL
  DEPENDS ${KERNEL_LST_NAME})

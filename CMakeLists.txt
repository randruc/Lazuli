# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Main CMake file to build the user project.
#

cmake_minimum_required(VERSION 3.12)

set(CMAKE_VERBOSE_MAKEFILE OFF)

add_subdirectory(sys)

# This defines the name of your project as well as its version and the languages
# it uses.
# You are free to change these values according to your project.
project(
  LazuliUserProject
  LANGUAGES C ASM
  VERSION 1.0.0)

# This lists all your source files.
# You are free to add source files according to your project.
set(
  LAZULI_USER_SOURCE_FILES
  user/main.c)

# This lists the compiler flags used to compile your project.
# You are free to change these flags according to your project.
set(
  LAZULI_USER_COMPILE_FLAGS
  -Wall
  -Wextra
  -Werror
  -O2
  -g
  # TODO: Manage this properly
  -mmcu=atmega328p)

# The following directives shouldn't be changed by the user
# TODO: maybe think about putting them into another file.

set(
  LZ_CONFIG_BUILD_UNIT_TESTS
  "null"
  CACHE STRING
  "Choice of unit tests set to build.")

set_property(
  CACHE LZ_CONFIG_BUILD_UNIT_TESTS
  PROPERTY STRINGS
  "null;unit_tests_1;unit_tests_2;unit_tests_3")

mark_as_advanced(LZ_CONFIG_BUILD_UNIT_TESTS)

set(
  LZ_EXAMPLE_PROGRAM
  "null"
  CACHE STRING
  "Choice of example program to build.")

set_property(
  CACHE LZ_EXAMPLE_PROGRAM
  PROPERTY STRINGS
  "null;blink.c;clock24.c;spinlocks.c;mutex.c;mutex_alternating_tasks.c")

if(NOT LZ_CONFIG_BUILD_UNIT_TESTS STREQUAL "null")
  set(
    LAZULI_USER_SOURCE_FILES
    sys/unit-tests/unit_tests_common.c
    sys/unit-tests/${LZ_CONFIG_BUILD_UNIT_TESTS}.c)
else()
  if(NOT LZ_EXAMPLE_PROGRAM STREQUAL "null")
    set(
      LAZULI_USER_SOURCE_FILES
      example-programs/${LZ_EXAMPLE_PROGRAM})
  endif()
endif()

include_directories(
  BEFORE
  sys/include                         # For Lazuli headers
  sys/libc-headers                    # For libc headers
  sys/libc-headers/arch-dependent/AVR # For arch-specific libc headers
  ${PROJECT_BINARY_DIR}/sys)          # For auto-generated config.h

set(
  LAZULI_USER_LINK_FLAGS
  -nostartfiles
  -nostdlib
  -nodefaultlibs
  #--gc-sections # This options is invalid when gcc is used as the linker
  -T${CMAKE_SOURCE_DIR}/sys/kern/arch/AVR/linker.ld
  -T${CMAKE_SOURCE_DIR}/sys/kern/linker.ld)

set(
  LAZULI_USER_PROJECT_NAME
  ${PROJECT_NAME}_${LZ_TARGET_MACHINE_CHOICE}_${CMAKE_PROJECT_VERSION})

add_executable(
  ${LAZULI_USER_PROJECT_NAME}
  ${LAZULI_USER_SOURCE_FILES})

set_target_properties(
  ${LAZULI_USER_PROJECT_NAME}
  PROPERTIES LINK_DEPENDS
  "${CMAKE_SOURCE_DIR}/sys/kern/arch/AVR/linker.ld;\
${CMAKE_SOURCE_DIR}/sys/kern/linker.ld")

target_link_libraries(
  ${LAZULI_USER_PROJECT_NAME}
  ${LAZULI_LIBRARY})

target_compile_options(
  ${LAZULI_USER_PROJECT_NAME}
  PUBLIC
  ${LAZULI_USER_COMPILE_FLAGS}
  -ffreestanding)

target_link_options(
  ${LAZULI_USER_PROJECT_NAME}
  PUBLIC ${LAZULI_USER_LINK_FLAGS})

# Generate disassembly listing.
set(LST_TARGET_NAME lst_output)

add_custom_command(
  OUTPUT
  ${LAZULI_USER_PROJECT_NAME}.lst
  COMMAND
  ${CMAKE_OBJDUMP} -hS ${LAZULI_USER_PROJECT_NAME}
  > ${LAZULI_USER_PROJECT_NAME}.lst
  DEPENDS
  ${LAZULI_USER_PROJECT_NAME}
  COMMENT "Generating user LST file: ${LAZULI_USER_PROJECT_NAME}.lst"
  VERBATIM)

add_custom_target(
  ${LST_TARGET_NAME}
  ALL
  DEPENDS ${LAZULI_USER_PROJECT_NAME}.lst)

add_dependencies(${LST_TARGET_NAME} ${LAZULI_USER_PROJECT_NAME})

# Generate HEX output
set(HEX_TARGET_NAME hex_output)

add_custom_command(
  OUTPUT
  ${LAZULI_USER_PROJECT_NAME}.hex
  COMMAND
  ${CMAKE_OBJCOPY}
  -j .text
  -j .data
  -j .rodata
  -j .progmem
  -O ihex
  ${LAZULI_USER_PROJECT_NAME}
  ${LAZULI_USER_PROJECT_NAME}.hex
  DEPENDS
  ${LAZULI_USER_PROJECT_NAME}
  COMMENT "Generating user HEX file: ${LAZULI_USER_PROJECT_NAME}.hex"
  VERBATIM)

add_custom_target(
  ${HEX_TARGET_NAME}
  ALL
  DEPENDS ${LAZULI_USER_PROJECT_NAME}.hex)

add_dependencies(${HEX_TARGET_NAME} ${LAZULI_USER_PROJECT_NAME})

# Extract final binary sections sizes
set(SIZE_TARGET_NAME size_output)

add_custom_command(
  OUTPUT
  ${LAZULI_USER_PROJECT_NAME}.size
  COMMAND
  ${MY_CMAKE_SIZE} -Adt ${LAZULI_USER_PROJECT_NAME}
  | ${CMAKE_SOURCE_DIR}/scripts/sizeof_sections.awk
  | tee ${LAZULI_USER_PROJECT_NAME}.size
  DEPENDS
  ${LAZULI_USER_PROJECT_NAME}
  COMMENT
  "Extracting final binary sections sizes: ${LAZULI_USER_PROJECT_NAME}.size"
  VERBATIM)

add_custom_target(
  ${SIZE_TARGET_NAME}
  ALL
  DEPENDS ${LAZULI_USER_PROJECT_NAME}.size)

add_dependencies(${SIZE_TARGET_NAME} ${LAZULI_USER_PROJECT_NAME})

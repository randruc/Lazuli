#
# Main CMake file to build the Lazuli kernel as a static library.
#
# Author: Remi Andruccioli
# Date: Sep 2019
#

cmake_minimum_required(VERSION 3.11)

set(CMAKE_VERBOSE_MAKEFILE OFF)

set(
  LAZULI_CORE_SOURCE_FILES
  kern/arch/AVR/arch.c
  kern/arch/AVR/interrupt_vectors_table.S
  kern/arch/AVR/startup.S
  kern/arch/AVR/timer_counter_1.c
  kern/kernel.c
  kern/memory.c
  kern/scheduler.c
  kern/list.c)

include(cmake/machine_choice.cmake)
include(cmake/initial_cache.cmake)
include(cmake/declare_lazuli_module.cmake)

project(
  LazuliKernel
  LANGUAGES C ASM
  VERSION 0.1.0)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  set(
    LZ_KERNEL_COMPILE_FLAGS
    -DLZ_DEBUG=1 # TODO: Remove this flag.
    -g           # TODO: Apply a condition for this flag.
    -Wall
    -Wextra
    -Werror
    -O2
    -ansi
    -std=c89
    -pedantic
    -fshort-enums
    -mmcu=atmega328p)
else()
  message(
    FATAL_ERROR
    "Fatal error: The compiler of ID ${CMAKE_C_COMPILER_ID} is unknown.")
endif()

include_directories(
  BEFORE
  include                         # For Lazuli headers
  libc-headers/arch-dependent/AVR # For libc headers
  ${PROJECT_BINARY_DIR})          # For auto-generated config.h

set(
  KERNEL_LIBRARY_NAME
  ${PROJECT_NAME}_${LZ_TARGET_MACHINE_CHOICE}_${PROJECT_VERSION})

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
  set(
    LAZULI_LIBRARY
    ${KERNEL_LIBRARY_NAME}
    PARENT_SCOPE)
endif()

# set(
#   KERNEL_LST_NAME
#   ${KERNEL_LIBRARY_NAME}.lst)

# This variable is a list containing all the declared modules.
# Elements will be added in this list by the subsequents modules declarations.
set(DECLARED_MODULES "")

add_subdirectory(kern/modules/clock_24)
add_subdirectory(kern/modules/mutex)
add_subdirectory(kern/modules/serial)
add_subdirectory(kern/modules/spinlock)
add_subdirectory(kern/modules/version_string)

configure_file(
  config.h.in
  config.h
  @ONLY
  NEWLINE_STYLE UNIX)

add_library(
  ${KERNEL_LIBRARY_NAME}
  STATIC
  ${LAZULI_CORE_SOURCE_FILES})

# Link with the activated modules
foreach(DECLARED_MODULE ${DECLARED_MODULES})
  string(TOUPPER ${DECLARED_MODULE} MODULE_NAME_UPPER)
  
  if(LZ_CONFIG_${MODULE_NAME_UPPER}_USED)
    message("Using module: ${DECLARED_MODULE}")
    
    target_sources(
      ${KERNEL_LIBRARY_NAME}
      PUBLIC
      $<TARGET_OBJECTS:${DECLARED_MODULE}>)
  endif()
endforeach()

target_compile_options(
  ${KERNEL_LIBRARY_NAME}
  PUBLIC ${LZ_KERNEL_COMPILE_FLAGS})

set(KERNEL_LST_TARGET_NAME kernel_lst_ouput)

add_custom_command(
  OUTPUT
  ${KERNEL_LIBRARY_NAME}.lst
  COMMAND
  ${CMAKE_OBJDUMP} -hS $<TARGET_FILE:${KERNEL_LIBRARY_NAME}>
  > ${KERNEL_LIBRARY_NAME}.lst
  DEPENDS
  ${KERNEL_LIBRARY_NAME}
  COMMENT "Generating kernel LST file: ${KERNEL_LIBRARY_NAME}.lst"
  VERBATIM)

add_custom_target(
  ${KERNEL_LST_TARGET_NAME}
  ALL
  DEPENDS ${KERNEL_LIBRARY_NAME}.lst)

add_dependencies(${KERNEL_LST_TARGET_NAME} ${KERNEL_LIBRARY_NAME})

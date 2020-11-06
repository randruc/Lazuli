# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2019-2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Definition of the CMake macro used to declare a Lazuli kernel module.
#

#
# Declare a lazuli kernel module.
# This is a variadic macro.
#
# The module name will be prefixed with "module_" by this macro.
# This macro registers a new object library target for the module, and defines
# the corresponding cache option (for enabling/disabling the module during
# configuration).
# The macro then adds the declared module in the global list of declared modules.
#
# @param NAME The module name, in lowercase, without prefixing it by "module_".
# @param SUMMARY A string describing the module functionality.
# @param SOURCES A list of the module source files.
#
macro(declare_lazuli_module)

  cmake_parse_arguments(
    # Prefix
    PARSED_ARGS
    # Option
    ""
    # Mono-values arguments
    "NAME;SUMMARY"
    # Multi-values arguments
    "SOURCES"
    # Arguments to parse
    ${ARGN})

  set(TARGET_MODULE_NAME module_${PARSED_ARGS_NAME})

  add_library(
    ${TARGET_MODULE_NAME}
    OBJECT
    ${PARSED_ARGS_SOURCES})

  target_compile_options(
    ${TARGET_MODULE_NAME}
    PRIVATE
    ${LZ_KERNEL_COMPILE_FLAGS})

  # Declare the configuration option (in cache)
  string(TOUPPER ${TARGET_MODULE_NAME} TARGET_MODULE_NAME_UPPER)

  option(
    LZ_CONFIG_${TARGET_MODULE_NAME_UPPER}_USED
    ${PARSED_ARGS_SUMMARY}
    OFF)

  # Add the module to the global list of declared modules
  set(DECLARED_MODULES ${DECLARED_MODULES} ${TARGET_MODULE_NAME} PARENT_SCOPE)
endmacro()

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Main CMake file for the printf module.
#

declare_lazuli_module(
  NAME printf

  SUMMARY "Module for formatted printing."

  SOURCES
  printf.c
  arch/AVR/printf.S)

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Main CMake file for the Clock 24 module.
#

declare_lazuli_module(
  NAME clock_24

  SUMMARY "Module implementing a 24-Hour clock in the kernel."

  SOURCES
  clock_24.c)

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Main CMake file for the division module.
#

declare_lazuli_module(
  NAME division

  SUMMARY "Module for integer division."

  SOURCES
  arch/AVR/division.S)

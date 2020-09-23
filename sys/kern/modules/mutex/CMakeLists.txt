# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Main CMake file for the Mutex module.
#

declare_lazuli_module(
  NAME mutex

  SUMMARY "Module for mutexes implementation."

  SOURCES
  mutex.c
  arch/AVR/mutex.S)

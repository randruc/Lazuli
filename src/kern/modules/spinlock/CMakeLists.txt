# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Main CMake file for the Spinlock module.
#

declare_lazuli_module(
  NAME spinlock

  SUMMARY "Module for spinlocks implementation."

  SOURCES
  spinlock.c
  arch/AVR/spinlock.S)

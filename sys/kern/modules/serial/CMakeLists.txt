# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Main CMake file for the Serial module.
#

declare_lazuli_module(
  NAME serial

  SUMMARY "Module for serial interface configuration."

  SOURCES
  serial.c
  arch/AVR/usart.c)

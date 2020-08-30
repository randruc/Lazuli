#! /bin/bash

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

# Open a serial connection to the Arduino serial interface.
# The user must be root.

screen /dev/ttyUSB0 19200

#!/bin/bash

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

# Upload the binary image to the target.
# The user must be root.

avrdude \
    -p m328p \
    -c atmelice_isp \
    -U lfuse:w:0xff:m \
    -U hfuse:w:0xd1:m \
    -U efuse:w:0xff:m \
    -U flash:w:$1

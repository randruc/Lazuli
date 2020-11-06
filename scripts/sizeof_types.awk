#!/usr/bin/awk -f

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# This script is used to output the final size (in bytes) of custom types.
#

$2 ~ /__sizeof/ { print $2; }
$4 ~ /word/ {
    converted_size = strtonum($5);
    printf("0x%x (%d)\n", converted_size, converted_size);
}

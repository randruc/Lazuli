#!/bin/bash

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2018-2020, Remi Andruccioli <remi.andruccioli@gmail.com>

# Execute clang-tidy on every .h and .c files.

clang-tidy \
    $(find . -iname *.h -o -iname *.c ! -name "unit_tests.c") \
    -checks=*,-readability-avoid-const-params-in-decls \
    -header-filter=* \
    -- \
    -Iinclude \
    -I../build/src \
    -Ilibc-headers/arch-dependent/x86_64 \
    -DLZ_STATIC_ANALYSIS=1

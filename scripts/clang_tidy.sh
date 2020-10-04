#!/bin/bash

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2018-2020, Remi Andruccioli <remi.andruccioli@gmail.com>

# Execute clang-tidy on every .h and .c files.

clang-tidy \
    $(find . \( -type f -name "*.h" -o -name "*.c" \) \
           ! -path "./sys/unit-tests/*" \
           ! -path "./build/*" ! -path "./templates/*") \
    -checks=*,-readability-avoid-const-params-in-decls \
    -header-filter=* \
    -- \
    -I./sys/include \
    -I./build/sys \
    -I./sys/libc-headers \
    -I./sys/libc-headers/arch-dependent/x86_64 \
    -DLZ_STATIC_ANALYSIS=1

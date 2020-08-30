#!/bin/bash

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Bash script to start a compilation.
#
# This script was initially written to be used with Emacs "compile" command,
# called through "docker exec" to start a compilation inside the container, from
# outside.
# See .dir-locals.el
#

cmake --build ./build

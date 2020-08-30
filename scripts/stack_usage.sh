#!/bin/bash

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Bash script to display the stack usage of the whole project nicely presented
# and sorted.
#

case $1 in
    -h)
        echo "$0 usage:"
        echo "    -h  Display help"
        echo "    -n  Order by file name (default)"
        echo "    -s  Order by stack size"
        ;;

    -s)
        find . -type f -name *.su | xargs cat | column -td | sort -rn -k2
        ;;

    -n|*)
        find . -type f -name *.su | xargs cat | column -td | sort -k1
        ;;
esac


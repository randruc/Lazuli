#!/bin/bash

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

case $1 in
    -n)
        objdump -t $2 |
            awk \
        '$3 ~ /^F$/ && $4 ~ /^\.text$/ { print strtonum("0x" $5) "\t" $6 }' |
            sort -f -k2 |
            column -td
        ;;

    -s)
        objdump -t $2 |
            awk \
        '$3 ~ /^F$/ && $4 ~ /^\.text$/ { print strtonum("0x" $5) "\t" $6 }' |
            sort -rn -k1 |
            column -td
        ;;

    -h|*)
        cat > /dev/tty <<EOF
$0
Display the size in bytes of each function of an object/executable file.
Usage:
$0 OPTION OBJECT
Where OPTION is one of the following:
    -h  Display help
    -n  Order by function name
    -s  Order by function size
Where OBJECT is the full path to an object/executable file.
EOF
        ;;
esac

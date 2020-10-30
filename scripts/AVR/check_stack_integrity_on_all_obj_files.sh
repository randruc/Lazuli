#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# This script prepares the input for check_stack_integrity.awk to check stack
# integrity on hand written AVR assembly code.
# This script relies on disassembly output from avr-objdump, so
# check_stack_integrity.awk operates on compiled code! By default all global ASM
# routines are checked, but it is possible to exclude routines from the default
# list, and manually include routines that are not globals.
#
# check_stack_integrity.awk takes its input on stdin. This input is divided into
# sections that start with a pattern. Here are the expected patterns/sections:
#
# @@@-FILENAME
# Followed by the object file name being checked.
#
# @@@-INCLUDE
# Followed by a list of routines names to be checked. This script will insert
# here all global routines defined in hand written ASM. The user can also
# manually insert non global ASM routines names.
#
# @@@-EXCLUDE
# Followed by a list of routines names to be excluded from checking.
# Excluded symbols have the priority over included ones.
#
# @@@-DISASSEMBLY
# Followed by the disassembly of the object file being checked
# (with avr-objdump -d).
#
# Any line starting with @@@-C is considered a comment until the end of the
# line.
#

for OBJ_FILE in $(find ./build -name "*.S.obj"); do
    {
        cat <<EOF
@@@-FILENAME
$OBJ_FILE
@@@-INCLUDE
copy_from_program_memory
$(avr-objdump -t -j .text $OBJ_FILE | awk '$2 ~ /g/ && $5 ~ /^[A-Z]/{print $5}')
@@@-EXCLUDE
Arch_RestoreContextAndReturnFromInterrupt
@@@-DISASSEMBLY
$(avr-objdump -d --no-show-raw-ins -j .text $OBJ_FILE)
EOF
    } | ./scripts/AVR/check_stack_integrity.awk;
done

#!/usr/bin/gawk -f
# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# This script is used to check the integrity of the stack on hand written AVR
# assembly code.
# It checks if all the registers that are pushed at the beginning of the routine
# are pop'd in the correct order before returning.
#
# This script takes its input from stdin, and displays errors on stdout.
# It is meant to be called by check_stack_integrity_on_all_obj_files.sh that
# prepares its input to stdint. See this script for a description of the
# expected input.
#

# Join elements of an array in a string with space separators.
function join(array)
{
    result = "";

    if (0 == stackIndex) {
        return result;
    }

    for (i = 0; i < stackIndex; ++i) {
        result = array[i] result;

        if (stackIndex >= 2 && i != stackIndex - 1) {
            result = " " result;
        }
    }

    return result;
}

# Return a dump of the current stack.
function dumpStack(array) {
    return sprintf("[top-> %s]", join(array));
}

# Print the file name if it wasn't already printed.
function printFileName() {
    if (!isFileNamePrinted) {
        print "In " fileName;

        isFileNamePrinted = 1;
    }
}

BEGIN {
    inFileName = 0;        # Flag if we are in the file name section.
    inExclusions = 0;      # Flag if we are in the exclusions list section.
    inInclusions = 0;      # Flag if we are in the inclusions list section.

    exitCode = 0;          # Exit code of the script.
    inRoutine = 0;         # Flag if we are currently reading a global routine.
    stackIndex = 0;        # The current index of the current stack's routine.
                           # Points to the next free stack element.

    isFileNamePrinted = 0; # Flag if the file name has been printed yet.
    fileName = "";         # The current file name.
    currentRoutine = "";   # The current routine name.
}

/^@@@-FILENAME$/ {
    inFileName = 1;
    inExclusions = 0;
    inInclusions = 0;
}

/^@@@-EXCLUDE$/ {
    inFileName = 0;
    inExclusions = 1;
    inInclusions = 0;
}

/^@@@-INCLUDE$/ {
    inFileName = 0;
    inExclusions = 0;
    inInclusions = 1;
}

/^@@@-DISASSEMBLY$/ {
    inFileName = 0;
    inExclusions = 0;
    inInclusions = 0;
}

# Comment
/^@@@-C/ {
    next;
}

inExclusions {
    exclusions[$0] = 1;
}

inInclusions {
    inclusions[$0] = 1;
}

inFileName {
    fileName = $0;
}

# Match the beginning of a global routine that is not in the exclusions list.
match($2, /<(.*)>:$/, groups) {
    if (!exclusions[groups[1]] && inclusions[groups[1]]) {
        inRoutine = 1;
        currentRoutine = groups[1];
        stackIndex = 0;
    }
}

# Match a register push.
inRoutine && $2 ~ /push/ {
    stack[stackIndex++] = $3;
}

# Match a return from routine or from interrupt.
inRoutine && $2 ~ /ret|reti/ {
    if (stackIndex != 0) {
        exitCode = 1;

        printFileName();

        printf( \
            "\t%s: Returning but there is still something on the stack %s.\n",
            currentRoutine,
            dumpStack(stack));
    }

    inRoutine = 0;
}

# Match a register pop.
inRoutine && $2 ~ /pop/ {
    if ($3 != stack[stackIndex - 1]) {
        exitCode = 1;

        printFileName();

        printf("\t%s: Trying to pop %s but %s was pushed %s.\n",
               currentRoutine,
               $3,
               0 == stackIndex ? "nothing" : stack[stackIndex - 1],
               dumpStack(stack));
    } else {
        if (stackIndex > 0) {
            --stackIndex;
        }
    }
}

# Return the appropriate exit code.
END {
    exit exitCode;
}

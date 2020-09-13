#!/usr/bin/gawk -f
# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# This script is used to parse the output of unit tests execution on the target
# machine and determine if they ran correctly.
#
# Reads from stdin.
# stdout: Error messages and/or executions status.
# stderr: Log of stdin (i.e. raw output from the target machine).
#
# When unit tests are run on the target machine, execution output is printed
# on the serial line. For obvious memory usage reasons, and to allow automation
# of unit tests runs, the output for the execution of unit tests must conform to
# the following specifications:
#
# * Test execution MUST start by outputting the sequence
#   ``--BEGIN tests:0123456789!``. This is to make sure the serial speed is
#   set correctly.
# * Any line starting with ``C:``, and in the form ``C:XXX:YYY``: Strings
#   ``XXX`` and ``YYY`` will be compared. An error will be output if they are
#   not equal.
# * Any line that displays a hexadecimal number, starting with ``0x`` and with
#   any number of digits: this indicates a test fail, and corresponds to the
#   line of the test fail. An error will be output showing the line number in
#   decimal.
# * A single ``.``: Means the end of the unit tests execution.
#
# TODO: Output an error if a pattern is not recognized.
#

# Print a summary of the unit tests execution.
function PrintSummary() {
    if (0 == failures) {
        print "Unit test suite is OK.";
    } else {
        printf("-------------------------------------------------\n");
        printf("Unit test suite has %d failure%s.\n",
               failures,
               failures > 1 ? "s" : "");
    }

    printf("Number of lines tested: %d.\n", NR);
}

# Return the appropriate program exit code, depending on the result of tests
# execution.
function GetExitCode() {
    if (0 == failures) {
        return 0;
    } else {
        return 1;
    }
}

BEGIN {
    RS = "\r\n";
    failures = 0;

    print "Reading test output..."

    # Now we test if the first line of output matches the expected sequence,
    # ignoring first blank lines if any.

    do {
        getline;
        print $0 > "/dev/stderr";
    } while (0 == NF);

    if ($0 != "--BEGIN tests:0123456789!") {
        failures++;
        printf("Test execution doesn't start with the expected sequence. ");
        printf("Aborting.\n");

        exit;
    }
}

# Every line is logged to stderr.
{
    print > "/dev/stderr";
}

# "C:": Compare two string, separated by ':'
match($0, /^C:([^:]*):([^:]*)/, groups) > 0 && groups[1] != groups[2] {
    failures++;

    printf("(%3d) Test fail: string comparison: \"%s\"\n", NR, $0);
}

# Hexadecimal number, indicates an error line number
/^0x([0-9]|[a-f]|[A-F])*$/ {
    failures++;

    printf("(%3d) Failed assertion on line: %d\n", NR, strtonum($0));
}

# End of unit tests execution.
/^.$/ {
    exit;
}

END {
    PrintSummary();

    exit GetExitCode();
}

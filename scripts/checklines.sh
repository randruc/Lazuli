#!/bin/bash

# SPDX-License-Identifier: GPL-3.0-only
# This file is part of Lazuli.
# Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

#
# Bash script to perform tests on lines of text.
#
# Include patterns for grep are sorted alphabetically.
#

failed=0

# Files with lines longer that 80 characters
tmp=$(mktemp)
grep -nrH \
     '.\{81\}'\
     --include=*.c \
     --include=*.dox \
     --include=*.h \
     --include=*.ld \
     --include=*.S \
     --include=*.sh \
     --include=*.txt \
     --exclude-dir=build \
     --exclude-dir=LICENSES \
     --exclude-dir=_build > $tmp
if [ $(cat $tmp | wc -l) -gt 0 ];
then
    failed=1;
    echo "Files with lines longer that 80 characters:"
    cat $tmp | awk '{print "    " $0}';
fi

# Files with tab characters
tmp=$(mktemp)
grep -nrHP \
     "\t" \
     --include=*.c \
     --include=*.cmake \
     --include=*.dox \
     --include=*.h \
     --include=*.ld \
     --include=*.md \
     --include=*.rst \
     --include=*.S \
     --include=*.sh \
     --include=*.txt \
     --include=*.yml \
     --exclude-dir=build \
     --exclude-dir=_build > $tmp
if [ $(cat $tmp | wc -l) -gt 0 ];
then
    failed=1;
    echo "Files with tab characters:"
    cat $tmp | awk '{print "    " $0}';
fi

# Files with trailing whitespaces
tmp=$(mktemp)
grep -nrH \
     "\([[:space:]]\)$" \
     --include=*.c \
     --include=*.cmake \
     --include=*.dox \
     --include=*.h \
     --include=*.ld \
     --include=*.S \
     --include=*.sh \
     --include=*.txt \
     --include=*.yml \
     --exclude-dir=build \
     --exclude-dir=_build > $tmp
if [ $(cat $tmp | wc -l) -gt 0 ];
then
    failed=1;
    echo "Files with trailing whitespaces:"
    cat $tmp | awk '{print "    " $0}';
fi

# Files with \r character
tmp=$(mktemp)
grep -nrH \
     $'\xd' \
     --exclude-dir=.git \
     --exclude-dir=_build \
     --exclude-dir=build \
     --exclude-dir=doxygen_output > $tmp
if [ $(cat $tmp | wc -l) -gt 0 ];
then
    failed=1;
    echo "Files with \r character:"
    cat $tmp | awk '{print "    " $0}';
fi

# Files with missing SPDX license identifier
tmp=$(mktemp)
grep -nHrL \
     "SPDX-License-Identifier: GPL-3.0-only" \
     --exclude-dir=.git \
     --exclude-dir=LICENSES \
     --exclude-dir=_build \
     --exclude-dir=build \
     --exclude-dir=doxygen_output \
     --exclude-dir=user \
     --exclude=COPYING \
     --exclude=TODO.org \
     --exclude=VERSION > $tmp
if [ $(cat $tmp | wc -l) -gt 0 ];
then
    failed=1;
    echo "Files with missing SPDX license identifier:"
    cat $tmp | awk '{print "    " $0}';
fi

# Files with missing copyright notice
tmp=$(mktemp)
grep -nHrL \
     "@copyright" \
     --include="*.c" \
     --include="*.h" \
     --include="*.S" \
     --exclude-dir=user \
     --exclude-dir=build > $tmp
if [ $(cat $tmp | wc -l) -gt 0 ];
then
    failed=1;
    echo "Files with missing copyright notice:"
    cat $tmp | awk '{print "    " $0}';
fi

exit $failed

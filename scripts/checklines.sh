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

echo "----- Files with lines longer that 80 characters -----"
! grep -nrH \
  '.\{81\}'\
  --color \
  --include=*.c \
  --include=*.dox \
  --include=*.h \
  --include=*.ld \
  --include=*.S \
  --include=*.sh \
  --include=*.txt \
  --exclude-dir=build \
  --exclude-dir=LICENSES \
  --exclude-dir=_build || failed=1

echo "----- Files with tab characters -----"
! grep -nrHP \
  --color \
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
  --exclude-dir=_build || failed=1

echo "----- Files with trailing whitespaces -----"
! grep -nrH \
  "\([[:space:]]\)$" \
  --color \
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
  --exclude-dir=_build || failed=1

# Find \r characters
# ! grep -nrHP \
#      "\r" \
#      --color \
#      --include=*.c \
#      --include=*.dox \
#      --include=*.h \
#      --include=*.ld \
#      --include=*.rst \
#      --include=*.S \
#      --include=*.sh \
#      --include=*.txt \
#      --exclude-dir=build \
#      --exclude-dir=_build || failed=1

echo "----- Files with missing SPDX license identifier: -----"
! grep -nHrL \
  "SPDX-License-Identifier: GPL-3.0-only" \
  --exclude-dir=.git \
  --exclude-dir=LICENSES \
  --exclude-dir=_build \
  --exclude-dir=build \
  --exclude-dir=doxygen_output \
  --exclude-dir=user \
  --exclude=COPYING \
  --exclude=TODO.org \
  --exclude=VERSION \
    || failed=1

echo "----- Files with missing copyright notice: -----"
! grep -nHrL \
  "@copyright" \
  --include="*.c" \
  --include="*.h" \
  --include="*.S" \
  --exclude-dir=user \
  --exclude-dir=build \
    || failed=1

exit $failed

#!/bin/bash

#
# Bash script to perform tests on lines of text.
#
# Include patterns for grep are sorted alphabetically.
#

failed=0

# Print lines longer than 80 characters
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
  --exclude-dir=_build || failed=1

# Find tab characters
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

# Find trailing whitespaces
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

exit $failed

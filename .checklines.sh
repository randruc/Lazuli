#!/bin/bash

filetypes="--include=*.c \
     --include=*.h \
     --include=*.sh \
     --include=*.ld \
     --include=*.S \
     --include=*.txt \
     --include=*.dox \
     --exclude-dir=build \
     --exclude-dir=_build"

failed=0

# Print lines longer than 80 characters
! grep -nrH \
     '.\{81\}'\
     --color \
     $filetypes || failed=1

# Find tab characters
! grep -nrHP \
     --color \
     "\t" \
     $filetypes || failed=1

# Find trailing whitespaces
! grep -nrH \
     "\([[:space:]]\)$" \
     --color \
     $filetypes || failed=1

exit $failed

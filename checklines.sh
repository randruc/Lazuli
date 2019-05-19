#!/bin/bash

filetypes="--include=*.c \
     --include=*.h \
     --include=*.md \
     --include=*.sh \
     --include=*.x \
     --include=*.S \
     --include=*.dox"

# Print lines longer than 80 characters
grep -nrH \
     '.\{81\}'\
     --color \
     $filetypes

# Find tab characters
grep -nrHP \
     --color \
     "\t" \
     $filetypes

# Find trailing whitespaces
grep -nrH \
     "\([[:space:]]\)$" \
     --color \
     $filetypes

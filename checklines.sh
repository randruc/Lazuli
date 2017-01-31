#!/bin/bash

# Print lines longer than 80 characters
grep -nrH \
     '.\{81\}'\
     --color \
     --include=*.c \
     --include=*.h \
     --include=*.md \
     --include=*.sh \
     --include=*.x \
     --include=*.S

grep -nrHP \
     --color \
     "\t" \
     --include=*.c \
     --include=*.h \
     --include=*.md \
     --include=*.sh \
     --include=*.x \
     --include=*.S

#!/bin/bash

# Print lines longer than 80 characters
grep -nrH \
	 '.\{81\}'\
	 --color \
	 --include=*.c \
	 --include=*.h \
	 --include=*.md \
	 --include=*.sh

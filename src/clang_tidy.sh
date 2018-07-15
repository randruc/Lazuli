#!/bin/bash

# Execute clang-tidy on every .h and .c files.

clang-tidy \
    $(find . -iname *.h -o -iname *.c) \
    -checks=*,-readability-avoid-const-params-in-decls \
    -header-filter=* \
    -- \
    -Iinclude \
    -Ilibc-headers/arch-dependent/x86_64

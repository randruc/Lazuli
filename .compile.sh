#!/bin/bash

#
# Bash script to start a compilation.
#
# This script was initially written to be used with Emacs "compile" command,
# called through "docker exec" to start a compilation inside the container, from
# outside.
# See .dir-locals.el
#

cmake --build ./build

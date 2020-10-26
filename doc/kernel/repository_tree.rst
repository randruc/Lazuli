..
   SPDX-License-Identifier: GPL-3.0-only
   This file is part of Lazuli.

.. sectionauthor::
   Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

Repository tree
===============

We give here an explanation of the main directorites of the repository.
It is displayed here from the root with the command ``tree -d`` with manual
modifications to show only the most important ones.
Comments on the right side have been added manually.

::

   .
   ├── doc                       Sources of Sphinx user documentation
   │   └── kernel                Sources of Sphinx kernel documentation
   ├── docker                    All files related to the Lazuli Docker image
   ├── example-programs          Sources of examples user programs that use the Lazuli API
   ├── LICENSES                  Text of the project's licenses
   ├── scripts                   Utility scripts
   ├── sys                       Base directory for all the system sources
   │   ├── cmake                 CMake files, referenced by CMakeLists.txt
   │   ├── include               Base directory of user and kernel header files
   │   │   └── Lazuli            Base directory of user and kernel header files
   │   │       └── sys           Directory of kernel header files
   │   ├── kern                  Base directory of kernel sources
   │   │   ├── arch              Base directory of arch-specific kernel sources
   │   │   └── modules           Base directory of kernel modules sources
   │   ├── libc                  Base directory of C files related to libc implementation
   │   ├── libc-headers          Base direcotry of standard C library header files
   │   │   └── arch-dependent    Base directory of arch-dependent libc header files
   │   └── unit-tests            Unit tests sources
   ├── templates                 File templates, used when creating new files
   └── user                      Directory for the user code.

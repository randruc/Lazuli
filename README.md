<!---
SPDX-License-Identifier: GPL-3.0-only
This file is part of Lazuli.
Copyright (c) 2017-2020, Remi Andruccioli <remi.andruccioli@gmail.com>
--->

![Lazuli CI Build](https://github.com/randruc/Lazuli/workflows/Lazuli%20CI%20Build/badge.svg)


# Lazuli


## What is Lazuli ?

Lazuli is a kernel designed for the AVR architecture.
It aims to provide preemptive time-sharing multitasking for AVR microcontrollers
(MCU).
It currently runs on the ATmega328p MCU (that is used on the Arduino) but it
should be easily portable to other AVR platforms.

The project is hosted on GitHub at https://github.com/randruc/Lazuli


### Features

* Multitasking of user defined tasks
* Realtime scheduling
* No MMU required
* "ROMable": all the system can fit in ROM
* Highly configurable, with a modular architecture
* A "wait for event" programming model
* A complete and containerized development environment


### Functionalities

For now, the Lazuli kernel provides the following functionalities:

* Rate Monotonic Scheduling
* Priority Round-robin Scheduling (POSIX's SCHED_RR)
* Mutexes
* AVR USART driver with a printf() API


### Challenge

The Lazuli project has very specific goals that make it unique. These are:

* __Free software__ - This project is developed with and can be built using only
  free software.
* __Pure C89__ - Lazuli is coded in pure C89 for all of its C parts.
  It means that no compiler-specific code nor extensions to the C language are
  used. This choice has been made the allow to code to be ported easily (if
  needed in the future) to many platforms and architectures, and to be compiled
  by the largest number of compilers. Another reason is that C89 is understood
  by all C code-checking tools.
* __Well documented__ - The Lazuli project aims to be a well-documented project.
  All the code is carefully documented and comes with good documentation about
  how it works and how to use it. If the documentation doesn't match the actual
  code or behavior, this is considered as a bug.


## Examples

Example programs that use the Lazuli RTOS can be found in the directory
[example-programs/](example-programs/).


## Documentation

The documentation for the project can be found in the [doc/](doc/) directory.

Two different tools are used to document the project:
* `sphinx` to generate user documentation from files written in reStructuredText.
* `doxygen` to generate API documentation from code comments..


## Prerequisites

Lazuli has no dependency on any other existing code.
You can simply write your own code, build the system, upload it and it just
runs!

To develop with Lazuli, it is strongly recommended to use the Lazuli Docker
image. You will benefit from a complete development environment, provided with
all the necessary tools.


## Configuration and Building

The Lazuli kernel is fully configurable.
The build system relies on CMake. With the help of `ccmake`, configuration can
also be done interactively in the console.

Read more in the documentation file
[doc/developing_your_project.rst](doc/developing_your_project.rst)


## Uploading binaries to the target platform

On AVR MCUs, `avrdude` can be used to uplaod the final binary to the target
machine.
The script [scripts/AVR/upload.sh](scripts/AVR/upload.sh) can be used for that.
It takes the HEX file as a parameter.

Interaction with the serial line can be done with the help of GNU `screen`.
The script [scripts/serial.sh](scripts/serial.sh) can be used to interact with
the USB serial line.


## Troubleshooting and improvements

The `Issues` tab (https://github.com/randruc/Lazuli/issues) of the project must
be used to report troubleshooting or to make suggestions.


## Contributing

__Contributions are welcome!__

The project is hosted on GitHub (at https://github.com/randruc/Lazuli), and
GitHub is used to manage it all: pull request, issues, etc.
It doesn't matter if you wish to fix a bug, implement or suggest new features,
or fix a spelling mistake: _any kind of contribution is welcome!_

Read more on how to contribute in the documentation file
[doc/kernel/contributing.rst](doc/kernel/contributing.rst).

You can also use the `Issues` tab of the project to ask questions, suggest
features without coding, or anything you wish!


## License

All the project is distributed under the GNU General Public License v3.0 only.
A full copy of this license is available in the file
[LICENSES/GPL-3.0-only.txt](LICENSES/GPL-3.0-only.txt).

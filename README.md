# Lazuli

![Lazuli CI Build](https://github.com/randruc/Lazuli/workflows/Lazuli%20CI%20Build/badge.svg)

## This is Lazuli

Lazuli is a kernel designed for the AVR architecture.
It aims to provide preemptive time-sharing multitasking for AVR MCUs.
It currently runs on the ATmega328p MCU (that is used on the Arduino) but it
should be easily portable to other AVR platforms.

The project is hosted on GitHub at https://github.com/randruc/Lazuli


### Features
* Multitasking of user defined tasks
* A "wait for event" programming model


### Challenge

The Lazuli project has very specific goals that make it unique. These are:
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
* __Free software__ - This project is developed and can be built using only
  free software.


## Prerequisites

Lazuli has no dependency on other piece of software.
You can simply build it, link it to your own code, upload it and it just
runs!

The software currently used is:
* __bash__ to run scripts
* __grep__ to process source files
* __avr-gcc__ to compile
* __avr-objcopy__ to produce hex binary image
* __avr-objdump__ to visualize C source code mixed with resulting asm (in debug)
* __ar__ to build the static library
* __avrdude__ to upload hex binary image to the target MCU
* __doxygen__ to build the documentation
* __awk__ to format output in the build process
* __size__ to display the size of sections in final binary image
* __git__ to manage source code


## Documentation

The documentation for the project can be found in the `doc/` directory.


## Building

Building is done by invoking the `build.sh` command in the `src/` directory.
There is no makefile for the moment (it's in the TODO list...).
The debug configuration is statically set in the `build.sh` file.
Invoking `build.sh` produces 2 files:
* An ELF file: `Lazuli.elf` which contains the resulting binary in ELF format.
* An HEX file: `Lazuli.hex` which is produced from `Lazuli.elf` and which
  contains binary code to be uploaded to the target MCU.


## Configuration


## Examples


## Uploading binaries to the target platform


## Contributing

__Contributions are welcome!__

The project is hosted on GitHub (at https://github.com/randruc/Lazuli), and
GitHub is used to manage it all, including issues and contributions.
It doesn't matter if you want to correct a bug, suggest
new features or correct a spelling mistake, _feel free to contribute!_

Before contributing, make sure you've read the `doc/AboutDocumentation.md` file.
If you wish to contribute to the code, make sure you've read the
`doc/CStyleAndConventions.md` file. Make also sure you've updated all the
appropriate files and lines of text apart from the code itself, including
comments, documentation, even this README file, or anything else.

To contribute:
* Fork the repository
* Create a branch that will bear your contribution
* Commit your contribution on your branch
* Perform a pull request

You can also use the `Issues` tab of the project to ask questions, suggest
features without coding, or anything you want!


### Troubleshooting

The `Issues` tab of the project must be used to report troubleshooting.


## Licensing

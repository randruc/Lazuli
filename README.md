# Lazuli

## This is Lazuli

Lazuli is a kernel designed for the AVR architecture.
It aims to provide preemptive time-sharing multitasking for AVR MCUs.
It currently runs on the ATmega328p MCU (that is used on the Arduino) but it
should be easily portable to other AVR platforms.

The project is hosted on GitHub at https://github.com/randruc/Lazuli


### Features


### Challenge

The Lazuli project has very specific goals that make it unique. These are:
* __Pure C89__ - Lazuli is coded in pure C89 for all of its C parts.
  It means that no compiler-specific code or extensions to the C language are
  used. This choice has been made the allow to code to be ported easily (if
  needed in the future) to many platforms and architecures, and to be compiled
  by the largest number of compilers. Another reason is that C89 is understood
  by nearly all C code-checking tools.
* __Public Domain__ - All the project is in the public domain.
* __Well documented__ - The Lazuli project aims to be a well-documented project.
  All the code is carrefully documented and comes with good documentation about
  how it works and how to use it.
* __Free software__ - This project is developed and can be built using only
  free software.


## Prerequisites

Lazuli has no dependency on other piece of software.
You can simply build it, link it to your own tasks, upload it and it just
runs!

## Documentation

The documentation for the project can be found in the `doc/` directory.


## Building


## Configuration


## Examples


## Uploading binaries to the target platform


## Contributing

__Contibutions are welcome!__

The project is hosted on GitHub (at https://github.com/randruc/Lazuli), and
GitHub is used to manage it all, including issues and contributions.
It doesn't matter if you want to correct a bug, suggest
new features or correct a spelling mistake, _feel free to contribute!_

Before contributing, make sure you're read the `doc/AboutDocumentation.md` file.
if you wish to contribute to the code, make sure you've read the
`doc/CStyleAndConventions.md` file. Make also sure you've updated all the
appropriate files and lines of text apart from the code itself , including
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


## Changelog

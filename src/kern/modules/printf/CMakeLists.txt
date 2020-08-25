#
# Main CMake file for the printf module.
#

declare_lazuli_module(
  NAME printf

  SUMMARY "Module for formatted printing."

  SOURCES
  printf.c
  arch/AVR/printf.S)

#
# Main CMake file for the Mutex module.
#

declare_lazuli_module(
  NAME mutex

  SUMMARY "Module for mutexes implementation."

  SOURCES
  mutex.c
  arch/AVR/mutex.S)

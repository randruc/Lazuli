#
# Main CMake file for the Mutex module.
#

declare_lazuli_module(
  mutex
  "Module for mutexes implementation."
  mutex.c
  arch/AVR/mutex.S)

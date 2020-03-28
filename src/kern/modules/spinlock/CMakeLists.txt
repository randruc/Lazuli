#
# Main CMake file for the Spinlock module.
#

declare_lazuli_module(
  spinlock
  OFF
  "Module for spinlocks implementation."
  spinlock.c
  arch/AVR/spinlock.S)

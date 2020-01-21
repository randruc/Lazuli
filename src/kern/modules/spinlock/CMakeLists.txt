#
# Main CMake file for the Spinlock module.
#

declare_lazuli_module(
  spinlock
  "Module for spinlocks implementation."
  spinlock.c
  arch/AVR/spinlock.S)

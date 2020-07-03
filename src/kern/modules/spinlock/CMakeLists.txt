#
# Main CMake file for the Spinlock module.
#

declare_lazuli_module(
  NAME spinlock

  SUMMARY "Module for spinlocks implementation."

  SOURCES
  spinlock.c
  arch/AVR/spinlock.S)

#
# Main CMake file for the Serial module.
#

declare_lazuli_module(
  NAME serial

  SUMMARY "Module for serial interface configuration."

  SOURCES
  serial.c
  arch/AVR/usart.c)

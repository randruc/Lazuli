#
# Main CMake file for the Serial module.
#

declare_lazuli_module(
  serial
  "Module for serial interface configuration."
  serial.c
  arch/AVR/usart.c)

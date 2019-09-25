#
# This file defines the cache variable "LZ_TARGET_MACHINE_CHOICE" used as a
# "drop-down" choice for the target
# machine.
#

# By default this variable is empty. This will bring an error at first-time
# cmake generation so we force the user to make a choice in order to load the
# appropriate toolchain file.

set(
  LZ_TARGET_MACHINE_CHOICE
  ""
  CACHE STRING
  "Choice of the target machine.")

set_property(
  CACHE LZ_TARGET_MACHINE_CHOICE
  PROPERTY STRINGS
  "AVR_ATmega328p")

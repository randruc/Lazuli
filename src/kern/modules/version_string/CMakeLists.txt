#
# Main CMake file for the version string module.
#

declare_lazuli_module(
  version_string
  ON
  "Module to include the Lazuli version string in final binary."
  version_string.c)

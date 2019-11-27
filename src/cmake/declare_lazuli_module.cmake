#
# Definition of the CMake macro used to declare a Lazuli kernel module.
#

#
# Declare a lazuli kernel module.
# This is a variadic macro.
#
# The module name will be prefixed with "module_" by this macro.
# This macro registers a new object library target for the module, and defines
# the corresponding cache option (for enabling/disabling the module during
# configuration).
# The macro then adds the declared module in the global list of declared modules.
#
# @param MODULE_NAME The module name, in lowercase, without prefixing it by
#                    "module_".
# @param MODULE_SUMMARY A summary of the module functionnality.
# @param ... A list of the module source files.
#
macro(declare_lazuli_module
    MODULE_NAME
    MODULE_SUMMARY
#   ...
    )

  set(TARGET_MODULE_NAME module_${MODULE_NAME})

  add_library(
    ${TARGET_MODULE_NAME}
    OBJECT
    ${ARGN})

  target_compile_options(
    ${TARGET_MODULE_NAME}
    PUBLIC
    ${LZ_KERNEL_COMPILE_FLAGS})
  
  # Declare the configuration option (in cache)
  string(TOUPPER ${TARGET_MODULE_NAME} TARGET_MODULE_NAME_UPPER)

  option(
    LZ_CONFIG_USE_${TARGET_MODULE_NAME_UPPER}
    ${MODULE_SUMMARY}
    OFF)

  # Add the module to the global list of declared modules
  set(DECLARED_MODULES ${DECLARED_MODULES} ${TARGET_MODULE_NAME} PARENT_SCOPE)
endmacro()

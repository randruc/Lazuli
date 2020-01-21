#
# CMake cross-compiling toolchain definitions for AVR targets.
#
# @author Remi Andruccioli
# @date Sep 2019
#

set(
  CMAKE_TRY_COMPILE_TARGET_TYPE
  STATIC_LIBRARY)

find_program(AVR_CC avr-gcc)
find_program(AVR_AR avr-ar)
find_program(AVR_NM avr-nm)
find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_OBJDUMP avr-objdump)

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_SYSTEM_PROCESSOR avr)

set(CMAKE_C_COMPILER ${AVR_CC} CACHE FILEPATH "AVR toolchain C compiler.")
set(CMAKE_ASM_COMPILER ${AVR_CC} CACHE FILEPATH "AVR toolchain assmebler.")
set(CMAKE_AR ${AVR_AR} CACHE FILEPATH "AVR toolchain static library archiver.")
set(CMAKE_LINKER ${AVR_CC} CACHE FILEPATH "AVR toolchain linker.")
set(CMAKE_NM ${AVR_NM} CACHE FILEPATH "AVR toolchain nm tool.")
set(CMAKE_OBJCOPY ${AVR_OBJCOPY} CACHE FILEPATH "AVR toolchain objcopy tool.")
set(CMAKE_OBJDUMP ${AVR_OBJDUMP} CACHE FILEPATH "AVR toolchain objdump tool.")

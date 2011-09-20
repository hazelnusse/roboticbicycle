include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME arm_cm3.cmake)
set(CMAKE_SYSTEM_PROCESSOR arm-none-eabi)

# Set the location of your ARM toolchain
set(ARM_NONE_EABI_DIR $ENV{HOME}/usr/arm_cm3)
#set(ARM_NONE_EABI_DIR $ENV{HOME}/CodeSourcery/Sourcery_G++_Lite)
set(CMAKE_FIND_ROOT_PATH ${ARM_NONE_EABI_DIR})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

CMAKE_FORCE_C_COMPILER(${ARM_NONE_EABI_DIR}/bin/arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(${ARM_NONE_EABI_DIR}/bin/arm-none-eabi-g++ GNU)

# Flags that are specific to the STM32 Connectivity line chip and are needed
# regardless of whether we are compiling C or C++ code.
SET(CM3_SPECIAL_FLAGS "-mcpu=cortex-m3 -mtune=cortex-m3 -march=armv7-m -mthumb")
# I don't think this option is needed: -mthumb-interwork")
# TODO: Figure out if Thumb-2 instructions are being properly generated.

SET(CMAKE_CXX_COMPILE_OBJECT "<CMAKE_CXX_COMPILER> ${CM3_SPECIAL_FLAGS} <DEFINES> <FLAGS> -o <OBJECT> -c <SOURCE>")
SET(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> ${CM3_SPECIAL_FLAGS} <DEFINES> <FLAGS> -o <OBJECT> -c <SOURCE>")
SET(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_C_COMPILER> ${CM3_SPECIAL_FLAGS} -x assembler-with-cpp <DEFINES> <FLAGS> -o <OBJECT> -c <SOURCE>")

set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> ${CM3_SPECIAL_FLAGS} <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -nostartfiles -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> ${CM3_SPECIAL_FLAGS} <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")

project(MCP ASM CXX C)
cmake_minimum_required(VERSION 2.8)

# Option to Build for different targets
set(STM32_Device "STM32F10X_CL" CACHE STRING "Target STM32 Device")
set_property(CACHE STM32_Device PROPERTY STRINGS STM32F10X_MD)

# Make "Debug" the default build option
set(CMAKE_BUILD_TYPE "Debug")

# gdb has 3 levels of debugging symbol support -- gdb3 includes all the symbols
# including those defined with preprocessor macros (#define)!!!  This means all
# the symbols in stm32f10x.h can be used directly within gdb
set(CMAKE_C_FLAGS_DEBUG "-Wall -fno-common -mcpu=cortex-m3 -march=armv7-m -mthumb -D'assert_param(expr)=((void)0)' -ggdb3 -O0 -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-exceptions")

set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

# Assembly Flags
set(CMAKE_ASM_FLAGS_DEBUG "-ggdb3 -mcpu=cortex-m3 -mthumb")

# Linker Flags
set(CMAKE_EXE_LINKER_FLAGS "-T${PROJECT_SOURCE_DIR}/src/${STM32_Device}.ld -nostartfiles")

# CMake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

# object-copy and object dump CMake macros
include(arm_macros)

# Add subdirectories
add_subdirectory(stm32)
add_subdirectory(src)

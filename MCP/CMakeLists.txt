project(MCP CXX C)
cmake_minimum_required(VERSION 2.8)

# Make "Debug" the default build option
set(CMAKE_BUILD_TYPE "Debug")

# gdb has 3 levels of debugging symbol support -- gdb3 includes all the symbols
# including those defined with preprocessor macros (#define)!!!  This means all
# the symbols in stm32f10x.h can be used directly within gdb
set(CMAKE_C_FLAGS_DEBUG "-ggdb3 -O0")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0")

# Option to Build for different targets
set(STM32_Device "STM32F10X_CL" CACHE STRING "Target STM32 Device")
set_property(CACHE STM32_Device PROPERTY STRINGS
    STM32F10X_LD STM32F10X_LD_VL
    STM32F10X_MD STM32F10X_MD_VL
    STM32F10X_HD STM32F10X_HD_VL
    STM32F10X_XL STM32F10X_CL)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

# object-copy and object dump CMake macros
include(arm_macros)

# Add subdirectories
add_subdirectory(stm32)
add_subdirectory(src)

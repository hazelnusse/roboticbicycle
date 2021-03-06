project(MCP ASM C CXX)
cmake_minimum_required(VERSION 2.8)
include(${PROJECT_SOURCE_DIR}/arm_macros.cmake)

# This will
set(STM32_Device STM32F10X_CL)

set(BUILD_TYPE "${STM32_Device}_flash")
# Uncomment to choose a ram build
#set(BUILD_TYPE "${STM32_Device}_ram")

MESSAGE("CMAKE_SYSTEM_INCLUDE_PATH=${CMAKE_SYSTEM_INCLUDE_PATH}")
MESSAGE("CMAKE_SYSTEM_LIBRARY_PATH=${CMAKE_SYSTEM_LIBRARY_PATH}")
MESSAGE("CMAKE_SYSTEM_PROGRAM_PATH=${CMAKE_SYSTEM_PROGRAM_PATH}")

# Make "Debug" the default build option
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")

# C and C++ flags
SET(C_CPP_FLAGS "-D'assert_param(expr)=((void)0)' -D${STM32_Device} -Wabi -Wall -Wextra -Wcast-align -Wpointer-arith -Wredundant-decls -Wshadow -Wcast-qual -W -Winline -finline-functions -ffunction-sections -fdata-sections")
# Assembler option dhlns

# C only flags
SET(C_ONLY_FLAGS "-Wimplicit -Wnested-externs -std=gnu99")
# C++ only Flags
SET(CPP_ONLY_FLAGS "-fno-rtti -fno-exceptions -std=c++0x -Weffc++")

# Debug flags
set(CMAKE_C_FLAGS_DEBUG "${C_CPP_FLAGS} ${C_ONLY_FLAGS} -ggdb3 -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${C_CPP_FLAGS} ${CPP_ONLY_FLAGS} -ggdb3 -O0")
set(CMAKE_ASM_FLAGS_DEBUG "-Wa,-ggdb3")

# Release flags
set(CMAKE_C_FLAGS_RELEASE "${C_CPP_FLAGS} ${C_ONLY_FLAGS} -ggdb3 -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${C_CPP_FLAGS} ${CPP_ONLY_FLAGS} -ggdb3 -O3")

# Executable linker Flags
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -T${PROJECT_SOURCE_DIR}/${BUILD_TYPE}.ld")

# Add subdirectories
add_subdirectory(stm32)
add_subdirectory(src)
add_subdirectory(FreeRTOS)

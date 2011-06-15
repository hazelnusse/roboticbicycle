project(MCP C CXX)
cmake_minimum_required(VERSION 2.8)

#include_directories(${PROJECT_SOURCE_DIR}/inc)

#add_definitions(-DSTM32F10X_MD)
# add_definitions(-DSTM32F10X_CL)

#set(CMAKE_C_FLAGS "-Wall -fno-common -mcpu=cortex-m3 -mthumb -D'assert_param(expr)=((void)0)'")
#set(CMAKE_CXX_FLAGS "-Wall -fno-common -mcpu=cortex-m3 -mthumb -D'assert_param(expr)=((void)0)'")
#set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -T${PROJECT_SOURCE_DIR}/scripts/stm_p103_blink_rom.cmd")

#add_executable(main main.c)
#add_executable(mainpp main.cpp)


add_subdirectory(stm32)
add_subdirectory(src)

project(MCP C CXX)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})
include(arm_macros)

add_subdirectory(stm32)
add_subdirectory(src)

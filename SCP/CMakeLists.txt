project(MCP CXX)
cmake_minimum_required(VERSION 2.8)

# Make "Debug" the default build option
set(CMAKE_BUILD_TYPE "Debug")

# C and C++ flags
SET(C_CPP_FLAGS "-Wall -Wextra -Wcast-align -Wpointer-arith -Wredundant-decls -Wshadow -Wcast-qual -W -Winline -fno-stack-protector -Wno-deprecated")
SET(CPP_ONLY_FLAGS "-std=gnu++0x")

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${C_CPP_FLAGS} ${CPP_ONLY_FLAGS} -ggdb3 -O0")

# Release flags
set(CMAKE_CXX_FLAGS_RELEASE "${C_CPP_FLAGS} ${CPP_ONLY_FLAGS} -ggdb3 -O3")
    
# Find Qt4
find_package(Qt4 REQUIRED)

find_package(VTK REQUIRED)

# Add subdirectories
add_subdirectory(src)

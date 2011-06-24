include(CMakeForceCompiler)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm-none-eabi)

# Set the location of your ARM toolchain
set(ARM_TC_DIR $ENV{HOME}/usr/sat)
set(CMAKE_FIND_ROOT_PATH ${ARM_TC_DIR})

# Tell CMAKE where to find the C/C++ compilers, it will automatically detect
# the names of all the other binutils
CMAKE_FORCE_C_COMPILER(${ARM_TC_DIR}/bin/arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(${ARM_TC_DIR}/bin/arm-none-eabi-g++ GNU)
set(CMAKE_ASM_COMPILER ${ARM_TC_DIR}/bin/arm-none-eabi-as)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

include(CMakeForceCompiler)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm-none-eabi)

# Set the location of your CodeSourcery/Sourcery_G++_Lite folder
set(ARM_TC_DIR $ENV{HOME}/usr/sat)
set(CMAKE_FIND_ROOT_PATH ${ARM_TC_DIR})

# Tell CMAKE where to find the C/C++ compilers, it will automatically detect
# the names of all the other binutils
CMAKE_FORCE_C_COMPILER(${ARM_TC_DIR}/bin/arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(${ARM_TC_DIR}/bin/arm-none-eabi-g++ GNU)
#set(CMAKE_C_COMPILER ${ARM_TC_DIR}/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER ${ARM_TC_DIR}/bin/arm-none-eabi-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Need to add OBJCOPY and OBJDUMP macros
#CPFLAGS = -Obinary
#ODFLAGS	= -S

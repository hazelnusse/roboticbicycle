include_directories(${PROJECT_SOURCE_DIR}/stm32/inc
                    ${PROJECT_SOURCE_DIR}/inc)

set(CMAKE_C_FLAGS "-Wall -fno-common -mcpu=cortex-m3 -mthumb -D'assert_param(expr)=((void)0)'")
set(CMAKE_CXX_FLAGS "-Wall -fno-common -mcpu=cortex-m3 -mthumb -D'assert_param(expr)=((void)0)'")
set(CMAKE_EXE_LINKER_FLAGS "-T${PROJECT_SOURCE_DIR}/scripts/stm3210x_cl.ld -nostartfiles ")

add_definitions(-DSTM32F10X_CL)

add_executable(blinky blinky.cpp)
objcopy_file(blinky)
objdump_file(blinky)

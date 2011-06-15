include_directories(${PROJECT_SOURCE_DIR}/stm32/inc
                    ${PROJECT_SOURCE_DIR}/inc)

set(CMAKE_C_FLAGS "-Wall -fno-common -mcpu=cortex-m3 -mthumb -D'assert_param(expr)=((void)0)'")
set(CMAKE_CXX_FLAGS "-Wall -fno-common -mcpu=cortex-m3 -mthumb -D'assert_param(expr)=((void)0)'")
set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -T${PROJECT_SOURCE_DIR}/scripts/stm_p103_blink_rom.cmd")

add_definitions(-DSTM32F10X_MD)
add_executable(main_c main.c)
objcopy_file(main_c)
objdump_file(main_c)
add_executable(main_cpp main.cpp)
objcopy_file(main_cpp)
objdump_file(main_cpp)

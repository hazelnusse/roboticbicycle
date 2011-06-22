include_directories(${PROJECT_SOURCE_DIR}/stm32/inc
                    ${PROJECT_SOURCE_DIR}/inc)

set(CMAKE_C_FLAGS "-Wall -fno-common -mcpu=cortex-m3 -mthumb -D'assert_param(expr)=((void)0)'")
set(CMAKE_CXX_FLAGS "-Wall -fno-common -mcpu=cortex-m3 -mthumb -D'assert_param(expr)=((void)0)'")

# Use linker script according to selected device
# Currently linker scripts for medium density and connectivity line devices
# have been created
set(CMAKE_EXE_LINKER_FLAGS "-T${PROJECT_SOURCE_DIR}/src/${STM32_Device}.ld -nostartfiles")

add_definitions("-D${STM32_Device}")

set_property(SOURCE startup_${STM32_Device}.s PROPERTY LANGUAGE C)
add_executable(test_startup test_startup.cpp startup_${STM32_Device}.s)
target_link_libraries(test_startup stm32)
objcopy_file(test_startup)
objdump_file(test_startup)

add_executable(blinky blinky.cpp)
objcopy_file(blinky)
objdump_file(blinky)

add_executable(pwm_test pwm_test.cpp)
objcopy_file(pwm_test)
objdump_file(pwm_test)

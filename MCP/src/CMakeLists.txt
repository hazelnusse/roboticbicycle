include_directories(${PROJECT_SOURCE_DIR}/stm32/inc
                    ${PROJECT_SOURCE_DIR}/inc
                    ${PROJECT_SOURCE_DIR}/src)

add_definitions("-D${STM32_Device}")

add_executable(test_startup test_startup.cc startup_${STM32_Device}.s)
target_link_libraries(test_startup stm32)
objcopy_file(test_startup)
objdump_file(test_startup)

add_executable(pwm_test pwm_test.cc startup_${STM32_Device}.s)
target_link_libraries(pwm_test stm32)
objcopy_file(pwm_test)
objdump_file(pwm_test)

add_executable(enc_test enc_test.cc startup_${STM32_Device}.s)
target_link_libraries(enc_test stm32)
objcopy_file(enc_test)
objdump_file(enc_test)

add_executable(spi_test spi_test.cc startup_${STM32_Device}.s)
target_link_libraries(spi_test stm32)
objcopy_file(spi_test)
objdump_file(spi_test)

add_definitions("-DITG_AD0=0")  # AD0 is grounded
add_definitions("-DADXL_ALT_ADDRESS=0")  # AD0 is grounded
add_executable(stickimu_test stickimu_test.cc startup_${STM32_Device}.s)
target_link_libraries(stickimu_test stm32)
objcopy_file(stickimu_test)
objdump_file(stickimu_test)

add_executable(floatingpoint_test floatingpoint_test.cc startup_${STM32_Device}.s)
target_link_libraries(floatingpoint_test stm32)
objcopy_file(floatingpoint_test)
objdump_file(floatingpoint_test)

add_executable(test_sample test_sample.cc sample.cc startup_${STM32_Device}.s)
target_link_libraries(test_sample stm32)
objcopy_file(test_sample)
objdump_file(test_sample)
symbol_table(test_sample)

# This uses new and delete and ends up bloating the code to about 60kb and
# requires a bunch of newlib system calls.... bad bad bad
add_executable(test_myclasses main.cpp myclasses.cpp syscalls.c startup_${STM32_Device}.s)
target_link_libraries(test_myclasses stm32)
objcopy_file(test_myclasses)
objdump_file(test_myclasses)
symbol_table(test_myclasses)

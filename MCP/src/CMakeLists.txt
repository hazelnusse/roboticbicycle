include_directories(${PROJECT_SOURCE_DIR}/stm32/inc
                    ${PROJECT_SOURCE_DIR}/inc)

add_definitions("-D${STM32_Device}")

add_executable(test_startup test_startup.cpp startup_${STM32_Device}.s)
target_link_libraries(test_startup stm32)
objcopy_file(test_startup)
objdump_file(test_startup)

add_executable(pwm_test pwm_test.cpp startup_${STM32_Device}.s)
target_link_libraries(pwm_test stm32)
objcopy_file(pwm_test)
objdump_file(pwm_test)

add_executable(enc_test enc_test.cpp startup_${STM32_Device}.s)
target_link_libraries(enc_test stm32)
objcopy_file(enc_test)
objdump_file(enc_test)

add_definitions("-DITG_AD0=0")  # AD0 is grounded
add_executable(spi_test spi_test.cpp startup_${STM32_Device}.s)
target_link_libraries(spi_test stm32)
objcopy_file(spi_test)
objdump_file(spi_test)

add_executable(stickimu_test stickimu_test.cpp startup_${STM32_Device}.s)
target_link_libraries(stickimu_test stm32)
objcopy_file(stickimu_test)
objdump_file(stickimu_test)

add_executable(floatingpoint_test floatingpoint_test.cpp startup_${STM32_Device}.s)
target_link_libraries(floatingpoint_test stm32)
objcopy_file(floatingpoint_test)
objdump_file(floatingpoint_test)

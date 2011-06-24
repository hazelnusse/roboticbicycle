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


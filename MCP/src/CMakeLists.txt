include_directories(${PROJECT_SOURCE_DIR}/stm32/inc
                    ${PROJECT_SOURCE_DIR}/FreeRTOS/inc
                    ${PROJECT_SOURCE_DIR}/inc
                    ${PROJECT_SOURCE_DIR}/src)

add_library(MCPtasks MCPtasks.cc)

add_executable(main main.cc sample.cc peripheral_init.cc startup_STM32F10X_CL.s)
target_link_libraries(main stm32 FreeRTOS MCPtasks)
objcopy_file(main)
objdump_file(main)
symbol_table(main)

add_subdirectory(test)

add_executable(steer_calibration steer_calibration.cc startup_STM32F10X_CL.s)
target_link_libraries(steer_calibration stm32)
objcopy_file(steer_calibration main)
objdump_file(steer_calibration main)
symbol_table(steer_calibration main)

#include_directories(${PROJECT_SOURCE_DIR}/stm32/inc
#                    ${PROJECT_SOURCE_DIR}/inc)
#                    ${PROJECT_SOURCE_DIR}/src)

#add_definitions("-D${STM32_Device}")

add_library(measurement sample.cc sample.h)

add_subdirectory(test)

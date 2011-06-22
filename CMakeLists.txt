project(roboticbicycle CXX C)
cmake_minimum_required(VERSION 2.8)

# Option to Build for different targets
set(STM32_Device "STM32F10X_CL" CACHE STRING "Target STM32 Device")
set_property(CACHE STM32_Device PROPERTY STRINGS STM32F10X_MD)
# Other options include:
#    STM32F10X_LD STM32F10X_LD_VL
#    STM32F10X_MD STM32F10X_MD_VL
#    STM32F10X_HD STM32F10X_HD_VL
#    STM32F10X_XL STM32F10X_CL)
# But linker scripts for these targets would also need to be created

add_subdirectory(MCP)

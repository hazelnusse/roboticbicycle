include_directories(${PROJECT_SOURCE_DIR}/stm32/inc)

#set(CMAKE_C_FLAGS "-Wall -fno-common -mcpu=cortex-m3 -mthumb -D'assert_param(expr)=((void)0)'")

# Define the target CPU, see stm32f10x.h for more details
add_definitions(-D${STM32_Device})

add_library(stm32
    core_cm3.c
    system_stm32f10x.c
    misc.c
    stm32f10x_adc.c
    stm32f10x_bkp.c
    stm32f10x_can.c
    stm32f10x_cec.c
    stm32f10x_crc.c
    stm32f10x_dac.c
    stm32f10x_dbgmcu.c
    stm32f10x_dma.c
    stm32f10x_exti.c
    stm32f10x_flash.c
    stm32f10x_fsmc.c
    stm32f10x_gpio.c
    stm32f10x_i2c.c
    stm32f10x_iwdg.c
    stm32f10x_pwr.c
    stm32f10x_rcc.c
    stm32f10x_rtc.c
    stm32f10x_sdio.c
    stm32f10x_spi.c
    stm32f10x_tim.c
    stm32f10x_usart.c
    stm32f10x_wwdg.c)

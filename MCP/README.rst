Master Control Progam
=====================

Outline of code organization

- inc/      header files
- src/      implementation files
- stm32/    ST Microelectronics Standard Peripheral Library
- FreeRTOS/  FreeRTOS real-time kernel
- Platform/  cmake Platform folder
- scripts/   openocd scripts

CMake Notes
===========

A CMake toolchain file is supplied (toolchain.cmake).  In order for the build
to proceed properly, you must invoke cmake as follows::

  $ cmake -DCMAKE_TOOLCHAIN_FILE=<MCP_dir>/toolchain.cmake <MCP_dir>

where <MCP_dir> is replaced with the actually path to the MCP directory that
contains toolchain.cmake.

OpenOCD Notes
=============

To connect to the board with openocd, you need to start openocd and give it the
script file located in scripts::

  $ openocd -f <MCP_dir>/scripts/openocd.cfg

Then, start up gdb:

  $ arm-none-eabi-gdb --eval-command="target remote localhost:3333"

Peripherals Used on STM32F107
-------------------------------

- SPI2:  SD Card
- USART1: XBee wireless radio
- TIM1:  PWM output to Motor controllers
- TIM3, TIM4, TIM5:  counters for steer, rear wheel, front wheel encoders
- GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, AFIO, some pins tied to I/O lines of above
  peripherals, others pins used for GPIO purposes

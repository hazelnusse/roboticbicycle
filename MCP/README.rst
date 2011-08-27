Master Control Progam
=====================

Outline of code organization

- inc/  header files
- src/  implementation files

CMake Notes
===========

A CMake toolchain file is supplied (toolchain.cmake).  In order for the build
to proceed properly, you must invoke cmake as follows::

  $ cmake -DCMAKE_TOOLCHAIN_FILE=<MCP_dir>/toolchain.cmake <MCP_dir>

where <MCP_dir> is replaced with the actually path to the MCP directory that
contains toolchain.cmake.


Peripherals Used on STM32F107
-------------------------------

- SPI1:  VN-100, 3 x SPI Digital Counters
- SPI2:  SD Card
- USART1: XBee wireless radio
- TIM2,3:  PWM output to Motor controllers

  - Must be on 5.0V tolerant pins
  - Outputs must be configured to open-drain
  - PWM frequency must be in 1-100kHz range.
  - Inputs on Copley motor controller must be pulled up to 5.0V by selecting that option in software

- 3 GPIO pins configured as open drain outputs to use as slave select
  pins for the SPI digital counters.
- 1 GPIO pin configured as input to use for DR_INT from VN-100



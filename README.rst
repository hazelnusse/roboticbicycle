=======================
Robotic Bicycle Control
=======================

Hardware
--------
The following is a list of the electrical hardware used on the bicycle:

- Steer Motor Controller: `Accelnet Panel ADP-090-36 <http://www.copleycontrols.com/motion/pdf/Accelnet_Panel_ADP.pdf>`_
- Rear hub motor Controller: `Accelnet Micro Panel ACJ-055-18 <http://www.copleycontrols.com/motion/pdf/Accelnet_Micro_Panel.pdf>`_
- Steer motor: `Teknic M-3441 <http://www.teknic.com/files/product_info/N34_Industrial_Grade_Motors_v3.2.pdf>`_
- Geared (4.7:1) rear hub motor: `Amped Bikes Rear Geared Kit <http://ampedbikes.com/kits.html>`_
- Wheel Encoders: `US Digital H5-50-N-D <http://usdigital.com/assets/general/119_h5_datasheet_1.pdf>`_
- Inertial Measurement Unit: `<http://www.sparkfun.com/products/10321>`_
- Microcontroller:
    - `Olimex STM32 H-107 <http://www.olimex.com/dev/pdf/ARM/ST/STM32-H107.pdf>`_
    - `STM32F107VC Datasheet <http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/CD00220364.pdf>`_
    - `STM32F107VC Reference Manual <http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/REFERENCE_MANUAL/CD00171190.pdf>`_
- Quadruple Differential Line Receiver: `TI AM26C32IN <http://www.ti.com/litv/pdf/slls104i>`_
- Octal Buffer/Driver: `TI SN74LVC244A <http://www.ti.com/lit/gpn/sn74lvc244a>`_
- JTAG Cable: `Olimex ARM-USB-TINY-H <http://www.olimex.com/dev/arm-usb-tiny-h.html>`_
- Wireless Radio: `XBee Pro <http://ftp1.digi.com/support/documentation/90000982_B.pdf>`_
- USB to RS232 Cable: `FTDI US232R-10 <http://www.ftdichip.com/Support/Documents/DataSheets/Cables/DS_US232R-10_R-100-500.pdf>`_
- USB to 3.3V TTL Cable: `TTL-232RG-VREG3V3-WE <http://www.ftdichip.com/Support/Documents/DataSheets/Cables/DS_TTL-232RG_CABLES.pdf>`_

Resources
---------
- `STM32/ARM Cortex-M3 HOWTO: Development under Ubuntu <http://fun-tech.se/stm32/index.php>`_
- `Sourcery G++ Lite 2011.03-42 for ARM EABI <http://www.codesourcery.com/sgpp/lite/arm/portal/release1802>`_
- `OpenOCD:  <http://openocd.berlios.de/web/>`_
- `STM32 Standand Peripherals Library: <http://www.st.com/internet/com/SOFTWARE_RESOURCES/SW_COMPONENT/FIRMWARE/stm32f10x_stdperiph_lib.zip>`_


GNU Toolchain
-------------
I've been using `Summon Arm Toolchain (SAT)Summon Arm Toolchain (SAT)
<http://github.com/esden/summon-arm-toolchain>`_ to build gcc, gdb, newlib,
binutils, and openocd.  The versions of all those tools in the master branch of
SAT work but some are a bit out of date.  With a bit of experimenting, I have
been using the following version of the toolchain:

- gdb-linaro-7.2-2011.05-0
- binutils-2.22.51
- newlib-1.19.0
- gcc-linaro-4.6-2011.10
- master branch of openocd

The only non open sourced part of this project is the STM32 standard peripheral
library.  It is free, but not OSI compliant.

Build System
------------
CMake is used for compilation.  CMake 2.8 or later is required.

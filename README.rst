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
- Inertial Measurement Unit:
    - `VectorNav VN-100 development board <http://www.vectornav.com/Downloads/Support/UM002.pdf>`_
    - `VectorNav VN-100 <http://www.vectornav.com/Downloads/Support/UM001.pdf>`_
    - `Embedded Firmware Library <http://www.vectornav.com/Downloads/Support/UM003.pdf>`_
- Microcontroller:
    - `Olimex STM32 P-103 <http://www.olimex.com/dev/pdf/ARM/ST/STM32-P103.pdf>`_
    - `STM32F103RB Datasheet <http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/CD00161566.pdf>`_
    - `STM32F103RB Reference Manual <http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/REFERENCE_MANUAL/CD00171190.pdf>`_
    - Controller Area Network (CAN) transciever `SN65HVD230D <http://focus.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=sn65hvd230&fileType=pdf>`_
- JTAG Cable: `Olimex ARM-USB-TINY-H <http://www.olimex.com/dev/arm-usb-tiny-h.html>`_
- Wireless Radio: `XBee Pro <http://ftp1.digi.com/support/documentation/90000982_B.pdf>`_
- SPI Quadrature Counters: `LS7366R <http://www.lsicsi.com/pdfs/Data_Sheets/LS7366R.pdf>`_
- USB to RS232 Cable: `FTDI US232R-10 <http://www.ftdichip.com/Support/Documents/DataSheets/Cables/DS_US232R-10_R-100-500.pdf>`_
- USB to 3.3V TTL Cable: `TTL-232RG-VREG3V3-WE <http://www.ftdichip.com/Support/Documents/DataSheets/Cables/DS_TTL-232RG_CABLES.pdf>`_

Resources
---------
- `STM32/ARM Cortex-M3 HOWTO: Development under Ubuntu <http://fun-tech.se/stm32/index.php>`_
- `Sourcery G++ Lite 2011.03-42 for ARM EABI <http://www.codesourcery.com/sgpp/lite/arm/portal/release1802>`_
- `OpenOCD:  <http://openocd.berlios.de/web/>`_
- `STM32 Standand Peripherals Library: <http://www.st.com/internet/com/SOFTWARE_RESOURCES/SW_COMPONENT/FIRMWARE/stm32f2xx_stdperiph_lib.zip>`_


GNU ARM Toolchain
-----------------
The most reliable option I have found is to install the Sourcery G++ Lite ARM
EABI toolchain mentioned above.  I just did the binary installer and all of the
tools just work out of the box.  There are a few alternatives that attempt to
build the whole toolchain on your machine, but I had compilation problems that
the maintainers were unable to resolve right away.  The options I have seen
are

- `Summon Arm Toolchain (SAT) <http://github.com/esden/summon-arm-toolchain>`_
- `FunTechCortexMX_gcc <http://github.com/jsiei97/FunTechCortexMX_gcc>`_

SAT worked for me under Ubunto 10.10, but something changed in 11.04 and it
doesn't get past compiling binutils.  If this changes, I'll update things here.

OpenOCD
-------
I installed openocd from the Ubuntu repositories::

$ sudo apt-get install openocd

Three configuration files for OpenOCD are installed by default when you install
the openocd package under Ubuntu.  For the Olimex STM32 P-103 and the Olimex
ARM-USB-TINY-H jtag programmer, the correct ones are:

- interface: /usr/share/openocd/scripts/interface/olimex-arm-usb-tiny-h.cfg
- target: /usr/share/openocd/scripts/board/olimex_stm32_h103.cfg
- cpu: /usr/share/openocd/scripts/cpu/stm32.cfg

Briefly, the way OpenOCD works is

- start the OpenOCD daemon with the proper configuration files
- telnet into localhost port 4444
- through telnet, issue commands the board, including flashing the board,
  reseting it, erasing memory, etc.

OpenOCD lets you use gdb to debug a program that is running on the the
board.  This is really cool since you don't have to learn a whole new
development environment -- you can just use the standard GNU tools, which work
solid.


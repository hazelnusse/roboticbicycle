#ifndef HMC5883L_REG_H
#define HMC5883L_REG_H

#include <cstdint>

namespace hmc5883l {

  // I2C Slave Address
  const uint8_t ADDR_W = 0x3C;     // 8-bit Write address
  const uint8_t ADDR_R = 0x3D;     // 8-bit Read address

  // Register definitions
  const uint8_t  CRA = 0x0;         // Configuration Register A
  const uint8_t  CRB = 0x1;         // Configuration Register B
  const uint8_t   MR = 0x2;         // Mode Register
  const uint8_t DXRA = 0x3;         // Data Output X MSB Register
  const uint8_t DXRB = 0x4;         // Data Output X LSB Register
  const uint8_t DYRA = 0x5;         // Data Output Y MSB Register
  const uint8_t DYRB = 0x6;         // Data Output Y LSB Register
  const uint8_t DZRA = 0x7;         // Data Output Z MSB Register
  const uint8_t DZRB = 0x8;         // Data Output Z LSB Register
  const uint8_t   SR = 0x9;         // Status Register
  const uint8_t  IRA = 10;          // Identification Register A
  const uint8_t  IRB = 11;          // Identification Register B
  const uint8_t  IRC = 12;          // Identification Register C

  // Individual bits of Configuration Register A      (Table 3)
  const uint8_t MS0 = 0x01;
  const uint8_t MS1 = 0x02;
  const uint8_t DO0 = 0x04;
  const uint8_t DO1 = 0x08;
  const uint8_t DO2 = 0x10;
  const uint8_t MA0 = 0x20;
  const uint8_t MA1 = 0x40;

  // Valid Minimum Data Output Rate                   (Table 5)
  const uint8_t Rate_0_75_HZ = 0x0;
  const uint8_t Rate_1_5_HZ  = DO0;
  const uint8_t Rate_3_0_HZ  = DO1;
  const uint8_t Rate_7_5_HZ  = DO0 | DO1;
  const uint8_t Rate_15_0_HZ = DO2;                   // Default
  const uint8_t Rate_30_0_HZ = DO2 | DO0;
  const uint8_t Rate_75_0_HZ = DO2 | DO1;

  // Valid Modes                                      (Table 7)
  const uint8_t NORMAL_MODE   = 0x0;                  // Default
  const uint8_t POS_BIAS_MODE = MS0;
  const uint8_t NEG_BIAS_MODE = MS1;

  // Measurement averaging                            (Table 4)
  const uint8_t MEASURE_AVG_1 = 0x0;
  const uint8_t MEASURE_AVG_2 = 0x20;
  const uint8_t MEASURE_AVG_4 = 0x40;
  const uint8_t MEASURE_AVG_8 = 0x60;

  // Individual Bits of Configuration Register B      (Table 7)
  const uint8_t GN0 = 0x20;
  const uint8_t GN1 = 0x40;
  const uint8_t GN2 = 0x80;

  // Gain Settings                                (Table 9)
  // Gain units are in counts/Gauss               // Input field range
  const uint8_t GAIN_1370 = 0x0;                  // +/-0.88Ga
  const uint8_t GAIN_1090 = GN0;                  // +/-1.3Ga (default)
  const uint8_t GAIN_820 = GN1;                   // +/-1.9Ga
  const uint8_t GAIN_660  = GN1 | GN0;            // +/-2.5Ga
  const uint8_t GAIN_440  = GN2;                  // +/-4.0Ga
  const uint8_t GAIN_390  = GN2 | GN0;            // +/-4.7Ga
  const uint8_t GAIN_330  = GN2 | GN1;            // +/-5.6Ga
  const uint8_t GAIN_230  = GN2 | GN1 | GN0;      // +/-8.1Ga (Not recommended)

  // Mode Register Bits                           (Table 10)
  const uint8_t MD0 = 0x01;
  const uint8_t MD1 = 0x02;

  // Valid Operation Modes                        (Table 12)
  const uint8_t MODE_CONTINUOUS = 0x0;
  const uint8_t MODE_SINGLE     = MD0;
  const uint8_t MODE_IDLE       = MD1;
  const uint8_t MODE_SLEEP      = MD0 | MD1;

}; // namespace hmc5843

#endif  // HMC5843_REG_H

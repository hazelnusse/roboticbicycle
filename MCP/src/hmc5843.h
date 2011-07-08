#ifndef HMC5843_H
#define HMC5843_H

#define HMC_ADDR_R ((uint8_t)0x3D)     // 8-bit Read address
#define HMC_ADDR_W ((uint8_t)0x3C)     // 8-bit Write address

// Register definitions
#define HMC_CRA    ((uint8_t)0x00)     // Configuration Register A
#define HMC_CRB    ((uint8_t)0x01)     // Configuration Register B
#define HMC_MR     ((uint8_t)0x02)     // Mode Register
#define HMC_DXRA   ((uint8_t)0x03)     // Data Output X MSB Register
#define HMC_DXRB   ((uint8_t)0x04)     // Data Output X LSB Register
#define HMC_DYRA   ((uint8_t)0x05)     // Data Output Y MSB Register
#define HMC_DYRB   ((uint8_t)0x06)     // Data Output Y LSB Register
#define HMC_DZRA   ((uint8_t)0x07)     // Data Output Z MSB Register
#define HMC_DZRB   ((uint8_t)0x08)     // Data Output Z LSB Register
#define HMC_SR     ((uint8_t)0x09)     // Status Register
#define HMC_IRA    ((uint8_t)0x0A)     // Identification Register A
#define HMC_IRB    ((uint8_t)0x0B)     // Identification Register B
#define HMC_IRC    ((uint8_t)0x0C)     // Identification Register C

// Individual bits of Configuration Register A      (Table 4)
#define MS0            ((uint8_t)0x01)
#define MS1            ((uint8_t)0x02)
#define DO0            ((uint8_t)0x04)
#define DO1            ((uint8_t)0x08)
#define DO2            ((uint8_t)0x10)

// Valid Minimum Data Output Rate                   (Table 6)
#define ONE_HALF_HZ    ((uint8_t)0x00)
#define      ONE_HZ    DO0
#define      TWO_HZ    DO1
#define     FIVE_HZ    (DO0 | DO1)
#define      TEN_HZ    DO2
#define      TEN_HZ    DO2
#define   TWENTY_HZ    (DO2 | DO0)
#define    FIFTY_HZ    (DO2 | DO1)

// Valid Modes                                      (Table 7)
#define NORMAL_MODE     ((uint8_t)0x00)
#define POS_BIAS_MODE   MS0
#define NEG_BIAS_MODE   MS1

// Individual Bits of Configuration Register B      (Table 8)
#define GN0            ((uint8_t)0x20)
#define GN1            ((uint8_t)0x40)
#define GN2            ((uint8_t)0x80)

// Gain Settings                                    (Table 10)
// Gain units are in counts/Gauss            // Input field range
#define GAIN_1620      ((uint8_t)0x00)       // +/-0.7Ga
#define GAIN_1300      GN0                   // +/-1.0Ga (default)
#define GAIN_970       GN1                   // +/-1.5Ga
#define GAIN_780      (GN1 | GN0)            // +/-2.0Ga
#define GAIN_530       GN2                   // +/-3.2Ga
#define GAIN_460      (GN2 | GN0)            // +/-3.8Ga
#define GAIN_390      (GN2 | GN1)            // +/-4.5Ga
#define GAIN_280      (GN2 | GN1 | GN0)      // +/-6.5Ga (Not recommended)

// Mode Register Bits                               (Table 11)
#define MD0         ((uint8_t)0x01)
#define MD1         ((uint8_t)0x02)

// Mode Valid Operation Modes
#define MODE_CONTINUOUS  ((uint8_t)0x00)
#define MODE_SINGLE      MD0
#define MODE_IDLE        MD1
#define MODE_SLEEP       (MD0 | MD1)

#endif

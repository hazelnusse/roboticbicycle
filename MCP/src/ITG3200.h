#ifndef ITG3200_H
#define ITG3200_H

#define ITG_ADDR_W              ((uint8_t)0x68)  // 8-bit Write address
#define ITG_ADDR_R              ((uint8_t)0x69)  // 8-bit Read address

// Register definitions
#define ITG_WHO_AM_I            ((uint8_t)0x00)
#define ITG_SMPLRT_DIV          ((uint8_t)0x15)
#define ITG_DLPF_FS             ((uint8_t)0x16)
#define ITG_INT_CFG             ((uint8_t)0x17)
#define ITG_INT_STATUS          ((uint8_t)0x1A)
#define ITG_TEMP_OUT_H          ((uint8_t)0x1B)
#define ITG_TEMP_OUT_L          ((uint8_t)0x1C)
#define ITG_GYRO_XOUT_H         ((uint8_t)0x1D)
#define ITG_GYRO_XOUT_L         ((uint8_t)0x1E)
#define ITG_GYRO_YOUT_H         ((uint8_t)0x1F)
#define ITG_GYRO_YOUT_L         ((uint8_t)0x20)
#define ITG_GYRO_ZOUT_H         ((uint8_t)0x21)
#define ITG_GYRO_ZOUT_L         ((uint8_t)0x22)
#define ITG_PWR_MGM             ((uint8_t)0x3E)

// Bit field definitions for configurable registers

// DLPF, Full Scale         (ITG_DLPF_FS)
// FS_SEL (bits 3:4) must be 0b11 for proper operation, so don't define any bits, and
// automatically set these bits when we define all the DLPF_CFG (bits 0:2) options
#define ITG_DLPF_256            ((uint8_t)0x18)
#define ITG_DLPF_188            ((uint8_t)0x19)
#define ITG_DLPF_98             ((uint8_t)0x1A)
#define ITG_DLPF_42             ((uint8_t)0x1B)
#define ITG_DLPF_20             ((uint8_t)0x1C)
#define ITG_DLPF_10             ((uint8_t)0x1D)
#define ITG_DLPF_5              ((uint8_t)0x1E)

// Interrupt Configuration (ITG_INT_CFG)
#define ITG_RAW_RDY_EN          ((uint8_t)0x01)  // Enable interrupt when data is available
#define ITG_ITG_RDY_EN          ((uint8_t)0x04)  // Enable interrupt when device is ready (PLL ready after changing clock source)
#define ITG_INT_ANYRD_2CLEAR    ((uint8_t)0x10)  // latch clear method, 1=any register read, 0=status register read only
#define ITG_LATCH_INT_EN        ((uint8_t)0x20)  // latch mode, 1=latch until interrupt is cleared, 0=50us pulse
#define ITG_OPEN                ((uint8_t)0x40)  // drive type for INT output pin, 1=open drain, 0=push-pull
#define ITG_ACTL                ((uint8_t)0x80)  // logic level for INT output pin, 1=active Low, 0=active high

// Interrupt Status
#define ITG_ITG_RDY             ((uint8_t)0x04)  // PLL Ready
#define ITG_RAW_DATA_RDY        ((uint8_t)0x01)  // Raw data is ready

// Power Management (ITG_PWR_MGM)
#define ITG_H_RESET             ((uint8_t)0x80)  // Reset device and internal registers to the power-up-default settings
#define ITG_H_SLEEP             ((uint8_t)0x40)  // Enable low power sleep mode
#define ITG_STBY_XG             ((uint8_t)0x20)  // Put gyro X in standby mode (1=standby, 0=normal)
#define ITG_STBY_YG             ((uint8_t)0x10)  // Put gyro Y in standby mode (1=standby, 0=normal)
#define ITG_STBY_ZG             ((uint8_t)0x08)  // Put gyro Z in standby mode (1=standby, 0=normal)

// Clock select (ITG_PWR_MGM)
#define ITG_CLK_SEL_INTERNAL    ((uint8_t)0x00)  // Internal oscillator (not recommended for stability reasons)
#define ITG_CLK_SEL_PLL_X_GYRO  ((uint8_t)0x01)  // PLL with X Gyro reference
#define ITG_CLK_SEL_PLL_Y_GYRO  ((uint8_t)0x02)  // PLL with Y Gyro reference
#define ITG_CLK_SEL_PLL_Z_GYRO  ((uint8_t)0x03)  // PLL with Z Gyro reference
#define ITG_CLK_SEL_PLL_32KHZ   ((uint8_t)0x04)  // External clock source of 32.678 kHz
#define ITG_CLK_SEL_PLL_19MHZ   ((uint8_t)0x05)  // External clock source of 19.2 MHz

#endif

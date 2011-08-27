#ifndef BMA180_REG_H
#define BMA180_REG_H

#include <cstdint>

namespace bma180 {

// I2C Slave Address Depends on state of SDO, pin 8
#if BMA_SDO == 0
  const uint8_t ADDR_W = 0x80;  // 8-bit Write address
  const uint8_t ADDR_R = 0x81;  // 8-bit Read address
#elif BMA_SDO == 1
  const uint8_t ADDR_W = 0x82;  // 8-bit Write address
  const uint8_t ADDR_R = 0x83;  // 8-bit Read address
#else
#error "Please #define BMA_SDO to be 0 or 1 in accorandance with the state of BMA180 pin 8"
#endif

  // Register definitions
  const uint8_t             chip_id   = 0x00;        // Chip ID
  const uint8_t             version   = 0x01;        // Version
  const uint8_t             acc_x_lsb = 0x02;        // X LSB
  const uint8_t             acc_x_msb = 0x03;        // X MSB
  const uint8_t             acc_y_lsb = 0x04;        // Y LSB
  const uint8_t             acc_y_msb = 0x05;        // Y MSB
  const uint8_t             acc_z_lsb = 0x06;        // Z LSB
  const uint8_t             acc_z_msb = 0x07;        // Z MSB
  const uint8_t                  temp = 0x08;        // Temperature

  const uint8_t           status_reg1 = 0x09;        // Status Register 1
  const uint8_t           status_reg2 = 0x0A;        // Status Register 2
  const uint8_t           status_reg3 = 0x0B;        // Status Register 3
  const uint8_t           status_reg4 = 0x0C;        // Status Register 4

  const uint8_t             ctrl_reg0 = 0x0D;        // Control Register 0
  const uint8_t             ctrl_reg1 = 0x0E;        // Control Register 1
  const uint8_t             ctrl_reg2 = 0x0F;        // Control Register 2
  const uint8_t                 reset = 0x10;        // Soft reset

  const uint8_t                bw_tcs = 0x20;        // Bandwidth / Temperature
  const uint8_t             ctrl_reg3 = 0x21;        // Control Register 3
  const uint8_t             ctrl_reg4 = 0x22;        // Control Register 4
  const uint8_t                    hy = 0x23;
  const uint8_t    slope_tapsens_info = 0x24;
  const uint8_t         high_low_info = 0x25;
  const uint8_t               low_dur = 0x26;
  const uint8_t              high_dur = 0x27;
  const uint8_t            tapsens_th = 0x28;
  const uint8_t                low_th = 0x29;
  const uint8_t               high_th = 0x2A;
  const uint8_t              slope_th = 0x2B;
  const uint8_t                   cd1 = 0x2C;
  const uint8_t                   cd2 = 0x2D;
  const uint8_t                 tco_x = 0x2E;
  const uint8_t                 tco_y = 0x2F;
  const uint8_t                 tco_z = 0x30;
  const uint8_t                gain_t = 0x31;
  const uint8_t                gain_x = 0x32;
  const uint8_t                gain_y = 0x33;
  const uint8_t                gain_z = 0x34;
  const uint8_t           offset_lsb1 = 0x35;
  const uint8_t           offset_lsb2 = 0x36;
  const uint8_t              offset_t = 0x37;
  const uint8_t              offset_x = 0x38;
  const uint8_t              offset_y = 0x39;
  const uint8_t              offset_z = 0x3A;

  // EEPROM registers, write only, after writing each byte, soft reset must be
  // performed
  const uint8_t             ee_bw_tcs = 0x40;        // Bandwidth / Temperature
  const uint8_t          ee_ctrl_reg3 = 0x41;        // Control Register 3
  const uint8_t          ee_ctrl_reg4 = 0x42;        // Control Register 4
  const uint8_t                 ee_hy = 0x43;
  const uint8_t ee_slope_tapsens_info = 0x44;
  const uint8_t      ee_high_low_info = 0x45;
  const uint8_t            ee_low_dur = 0x46;
  const uint8_t           ee_high_dur = 0x47;
  const uint8_t         ee_tapsens_th = 0x48;
  const uint8_t             ee_low_th = 0x49;
  const uint8_t            ee_high_th = 0x4A;
  const uint8_t           ee_slope_th = 0x4B;
  const uint8_t                ee_cd1 = 0x4C;
  const uint8_t                ee_cd2 = 0x4D;
  const uint8_t              ee_tco_x = 0x4E;
  const uint8_t              ee_tco_y = 0x4F;
  const uint8_t              ee_tco_z = 0x50;
  const uint8_t             ee_gain_t = 0x51;
  const uint8_t             ee_gain_x = 0x52;
  const uint8_t             ee_gain_y = 0x53;
  const uint8_t             ee_gain_z = 0x54;
  const uint8_t        ee_offset_lsb1 = 0x55;
  const uint8_t        ee_offset_lsb2 = 0x56;
  const uint8_t           ee_offset_t = 0x57;
  const uint8_t           ee_offset_x = 0x58;
  const uint8_t           ee_offset_y = 0x59;
  const uint8_t           ee_offset_z = 0x5A;
  const uint8_t                ee_crc = 0x5B;

  // Individual bits of various registers
  // Status Register 1
  const uint8_t           ee_write = 0x01;
  const uint8_t        offset_st_s = 0x02;
  const uint8_t   slope_sign_z_int = 0x04;
  const uint8_t   slope_sign_y_int = 0x08;
  const uint8_t   slope_sign_x_int = 0x10;
  const uint8_t              alert = 0x20;
  const uint8_t    first_tapsens_s = 0x80;

  // Status Register 2
  const uint8_t     low_sign_z_int = 0x01;
  const uint8_t     low_sign_y_int = 0x02;
  const uint8_t     low_sign_x_int = 0x04;
  const uint8_t          tapsens_s = 0x10;
  const uint8_t            slope_s = 0x20;
  const uint8_t           low_th_s = 0x40;
  const uint8_t          high_th_s = 0x80;

  // Status Register 3
  const uint8_t        z_first_int = 0x01;
  const uint8_t        y_first_int = 0x02;
  const uint8_t        x_first_int = 0x04;
  const uint8_t        tapsens_int = 0x10;
  const uint8_t        slope_int_s = 0x20;
  const uint8_t         low_th_int = 0x40;
  const uint8_t        high_th_int = 0x80;

  // Status Register 4
  const uint8_t tapsens_sign_z_int = 0x04;
  const uint8_t tapsens_sign_y_int = 0x08;
  const uint8_t tapsens_sign_x_int = 0x10;
  const uint8_t    high_sign_z_int = 0x20;
  const uint8_t    high_sign_y_int = 0x40;
  const uint8_t    high_sign_x_int = 0x80;

  // Control Register 0
  const uint8_t        dis_wake_up = 0x01;
  const uint8_t              sleep = 0x02;
  const uint8_t                st0 = 0x04;
  const uint8_t                st1 = 0x08;
  const uint8_t               ee_w = 0x10;
  const uint8_t       update_image = 0x20;
  const uint8_t          reset_int = 0x40;

  // Control Register 1
  const uint8_t        en_offset_z = 0x20;
  const uint8_t        en_offset_y = 0x40;
  const uint8_t        en_offset_x = 0x80;

  // Temperature Compensation bits
  const uint8_t          tcs_n_4_0 = 0x00;
  const uint8_t          tcs_n_3_5 = 0x01;
  const uint8_t          tcs_n_3_0 = 0x02;
  const uint8_t          tcs_n_2_5 = 0x03;
  const uint8_t          tcs_n_2_0 = 0x04;
  const uint8_t          tcs_n_1_5 = 0x05;
  const uint8_t          tcs_n_1_0 = 0x06;
  const uint8_t          tcs_n_0_5 = 0x07;
  const uint8_t          tcs_0_0   = 0x08;
  const uint8_t          tcs_p_0_5 = 0x09;
  const uint8_t          tcs_p_1_0 = 0x0A;
  const uint8_t          tcs_p_1_5 = 0x0B;
  const uint8_t          tcs_p_2_0 = 0x0C;
  const uint8_t          tcs_p_2_5 = 0x0D;
  const uint8_t          tcs_p_3_0 = 0x0E;
  const uint8_t          tcs_p_3_5 = 0x0F;

  // Bandwidth bits
  const uint8_t              bw_10 = 0x00;
  const uint8_t              bw_20 = 0x10;
  const uint8_t              bw_40 = 0x20;
  const uint8_t              bw_75 = 0x30;
  const uint8_t             bw_150 = 0x40;
  const uint8_t             bw_300 = 0x50;
  const uint8_t             bw_600 = 0x60;
  const uint8_t            bw_1200 = 0x70;
  const uint8_t       bw_high_pass = 0x80;
  const uint8_t       bw_band_pass = 0x90;

  // Control Register 3, interrupt register
  const uint8_t            lat_int = 0x01;  // Affects high, low, slope and tap
  const uint8_t       new_data_int = 0x02;  // Set this bit
  const uint8_t            adv_int = 0x04;  // Set to zero, masks slope_* int
  const uint8_t        tapsens_int = 0x08;  // Set to zero
  const uint8_t            low_int = 0x10;  // Set to zero
  const uint8_t           high_int = 0x20;  // Set to zero
  const uint8_t          slope_int = 0x40;  // Set to zero
  const uint8_t        slope_alert = 0x80;  // 

  // Control Register 4

  // Slope Tap Sense Info
  const uint8_t       tapsens_filt = 0x01;
  const uint8_t      tapsens_int_z = 0x02;
  const uint8_t      tapsens_int_y = 0x04;
  const uint8_t      tapsens_int_x = 0x08;
  const uint8_t         slope_filt = 0x10;
  const uint8_t        slope_int_z = 0x20;
  const uint8_t        slope_int_y = 0x40;
  const uint8_t        slope_int_x = 0x80;

  // High Low Info
  const uint8_t           low_filt = 0x01;
  const uint8_t          low_int_z = 0x02;
  const uint8_t          low_int_y = 0x04;
  const uint8_t          low_int_x = 0x08;
  const uint8_t          high_filt = 0x10;
  const uint8_t         high_int_z = 0x20;
  const uint8_t         high_int_y = 0x40;
  const uint8_t         high_int_x = 0x80;

  // Low Duration
  const uint8_t          tco_range = 0x01;  // Temperature compensation

  // High Duration
  const uint8_t            dis_i2c = 0x01;

  const uint8_t      readout_12bit = 0x01;

  // Range bits
  const uint8_t         plus_minus_1g = 0x00;
  const uint8_t       plus_minus_1_5g = 0x02;
  const uint8_t         plus_minus_2g = 0x04;
  const uint8_t         plus_minus_3g = 0x06;
  const uint8_t         plus_minus_4g = 0x08;
  const uint8_t         plus_minus_8g = 0x0A;
  const uint8_t        plus_minus_16g = 0x0C;

  // Mode configuration
  const uint8_t               low_noise_mode = 0x00; // Default
  const uint8_t               low_power_mode = 0x03;

  // Sample skipping (offset_lsb1)
  const uint8_t         smp_skip = 0x01;
  // Wake up (gain_z)
  const uint8_t         wake_up = 0x01;
  // Shadow Disable (gain_y)
  const uint8_t         shadow_dis = 0x01;
  // Disable regulator (gain_x)
  const uint8_t         dis_reg = 0x01;         // Set to zero

  // Internal update frequency
  const uint32_t  Update_freq = 2400;          // TUpdate = 1/freq

}; // namespace bma180

#endif  // BMA180_REG_H

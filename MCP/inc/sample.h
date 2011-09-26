#ifndef SAMPLE_H
#define SAMPLE_H

#include <cstdint>

class Sample {
 public :
  // Gyroscope Measurements
  int16_t gyroT(void) const;             // Getter
  int16_t gyroX(void) const;             // Getter
  int16_t gyroY(void) const;             // Getter
  int16_t gyroZ(void) const;             // Getter
  void gyroTXYZ(int16_t ar[4]) const;    // Getter
  void gyroT(const int16_t val);         // Setter
  void gyroX(const int16_t val);         // Setter
  void gyroY(const int16_t val);         // Setter
  void gyroZ(const int16_t val);         // Setter
  void gyroTXYZ(const int16_t ar[4]);    // Setter

  // Accelerometer Measurements
  int16_t accX(void) const;              // Getter
  int16_t accY(void) const;              // Getter
  int16_t accZ(void) const;              // Getter
  void accXYZ(int16_t ar[3]) const;      // Getter
  void accX(const int16_t val);          // Setter
  void accY(const int16_t val);          // Setter
  void accZ(const int16_t val);          // Setter
  void accXYZ(const int16_t ar[3]);      // Setter
  
  // Magnetometer Measurements
  int16_t magX(void) const;              // Getter
  int16_t magY(void) const;              // Getter
  int16_t magZ(void) const;              // Getter
  void magXYZ(int16_t ar[3]) const;      // Getter
  void magX(const int16_t val);          // Setter
  void magY(const int16_t val);          // Setter
  void magZ(const int16_t val);          // Setter
  void magXYZ(const int16_t ar[3]);      // Setter

  // Steer Measurement
  uint16_t Steer(void) const;            // Getter
  void Steer(uint16_t val);              // Setter
  
  // Rear Wheel Measurement
  uint16_t RearWheel(void) const;        // Getter
  void RearWheel(uint16_t val);          // Setter

  // Front Wheel Measurement
  uint16_t FrontWheel(void) const;       // Getter
  void FrontWheel(uint16_t val);         // Setter

  // Sample Time
  void Time(uint32_t val);               // Setter
  uint32_t Time(void) const;             // Getter

  // Extra information data
  int16_t Extra(void) const;	           // Getter
  void Extra(int16_t val);	             // Setter

  // Overload operator[]
  uint8_t operator[](uint8_t i) const;   // Getter
  uint8_t &operator[](uint8_t i);        // Setter

 private :
  uint8_t data[32];
  // Store data in the byte order that it will be clocked out of each device,
  // first gyroscope, then accelerometer, then magnetometer.
  // This will allow for DMA transfers to populate each sample instance.
  enum {gt_h = 0, gt_l, gx_h, gx_l, gy_h, gy_l, gz_h, gz_l,
        ax_l, ax_h, ay_l, ay_h, az_l, az_h, 
        mx_h, mx_l, my_h, my_l, mz_h, mz_l, 
        st_h, st_l, rw_h, rw_l, fw_h, fw_l, 
        t_hh, t_hl, t_lh, t_ll, e_h, e_l};
};

// Gyroscope Getters
inline
int16_t Sample::gyroT(void) const
{
  return int16_t((data[gt_h] << 8) | data[gt_l]);
}

inline
int16_t Sample::gyroX(void) const
{
  return int16_t((data[gx_h] << 8) | data[gx_l]);
}

inline
int16_t Sample::gyroY(void) const
{
  return int16_t((data[gy_h] << 8) | data[gy_l]);
}

inline
int16_t Sample::gyroZ(void) const
{
  return int16_t((data[gz_h] << 8) | data[gz_l]);
}

inline
void Sample::gyroTXYZ(int16_t ar[4]) const
{
  ar[0] = gyroT();
  ar[1] = gyroX();
  ar[2] = gyroY();
  ar[3] = gyroZ();
}

// Gyroscope Setters
inline
void Sample::gyroT(const int16_t val)
{
  data[gt_h] = uint8_t(val >> 8);
  data[gt_l] = uint8_t(val);
}

inline
void Sample::gyroX(const int16_t val)
{
  data[gx_h] = uint8_t(val >> 8);
  data[gx_l] = uint8_t(val);
}

inline
void Sample::gyroY(const int16_t val)
{
  data[gy_h] = uint8_t(val >> 8);
  data[gy_l] = uint8_t(val);
}

inline
void Sample::gyroZ(const int16_t val)
{
  data[gz_h] = uint8_t(val >> 8);
  data[gz_l] = uint8_t(val);
}

inline
void Sample::gyroTXYZ(const int16_t ar[4])
{
  gyroT(ar[0]);
  gyroX(ar[1]);
  gyroY(ar[2]);
  gyroZ(ar[3]);
}

// Accelerometer Getters
inline
int16_t Sample::accX(void) const
{
  return int16_t((data[ax_h] << 8) | data[ax_l]);
}

inline
int16_t Sample::accY(void) const
{
  return int16_t((data[ay_h] << 8) | data[ay_l]);
}

inline
int16_t Sample::accZ(void) const
{
  return int16_t((data[az_h] << 8) | data[az_l]);
}

inline
void Sample::accXYZ(int16_t ar[3]) const
{
  ar[0] = accX();
  ar[1] = accY();
  ar[2] = accZ();
}

// Accelerometer Setters
inline
void Sample::accX(const int16_t val)
{
  data[ax_h] = uint8_t(val >> 8);
  data[ax_l] = uint8_t(val);
}

inline
void Sample::accY(const int16_t val)
{
  data[ay_h] = uint8_t(val >> 8);
  data[ay_l] = uint8_t(val);
}

inline
void Sample::accZ(const int16_t val)
{
  data[az_h] = uint8_t(val >> 8);
  data[az_l] = uint8_t(val);
}

inline
void Sample::accXYZ(const int16_t ar[3])
{
  accX(ar[0]);
  accY(ar[1]);
  accZ(ar[2]);
}

// Magnetometer getters
inline
int16_t Sample::magX(void) const
{
  return int16_t((data[mx_h] << 8) | data[mx_l]);
}

inline
int16_t Sample::magY(void) const
{
  return int16_t((data[my_h] << 8) | data[my_l]);
}

inline
int16_t Sample::magZ(void) const
{
  return int16_t((data[my_h] << 8) | data[my_l]);
}

inline
void Sample::magXYZ(int16_t ar[3]) const
{
  ar[0] = magX();
  ar[1] = magY();
  ar[2] = magZ();
}

// Magnetometer setters
inline
void Sample::magX(const int16_t val)
{
  data[mx_h] = uint8_t(val >> 8);
  data[mx_l] = uint8_t(val);
}

inline
void Sample::magY(const int16_t val)
{
  data[my_h] = uint8_t(val >> 8);
  data[my_l] = uint8_t(val);
}

inline
void Sample::magZ(const int16_t val)
{
  data[mz_h] = uint8_t(val >> 8);
  data[mz_l] = uint8_t(val);
}

inline
void Sample::magXYZ(const int16_t ar[3])
{
  magX(ar[0]);
  magY(ar[1]);
  magZ(ar[2]);
}

inline
void Sample::Time(uint32_t val)
{
  data[t_hh] = uint8_t(val >> 24);
  data[t_hl] = uint8_t(val >> 16);
  data[t_lh] = uint8_t(val >> 8);
  data[t_ll] = uint8_t(val);
}

inline
void Sample::Extra(int16_t val)
{
  data[e_h] = uint8_t(val >> 8);
  data[e_l] = uint8_t(val);
}

inline
void Sample::Steer(uint16_t val)
{
  data[st_h] = uint8_t(val >> 8);
  data[st_l] = uint8_t(val);
}

inline
void Sample::RearWheel(uint16_t val)
{
  data[rw_h] = uint8_t(val >> 8);
  data[rw_l] = uint8_t(val);
}

inline
void Sample::FrontWheel(uint16_t val)
{
  data[fw_h] = uint8_t(val >> 8);
  data[fw_l] = uint8_t(val);
}

inline
uint16_t Sample::Steer(void) const
{
  return uint16_t((data[st_h] << 8) | data[st_l]);
}

inline
uint16_t Sample::RearWheel(void) const
{
  return uint16_t((data[rw_h] << 8) | data[rw_l]);
}

inline
uint16_t Sample::FrontWheel(void) const
{
  return uint16_t((data[fw_h] << 8) | data[fw_l]);
}

inline
uint32_t Sample::Time(void) const
{
  return uint32_t((data[27] << 24)
                | (data[28] << 16)
                | (data[29] << 8)
                |  data[30]);
}

inline
int16_t Sample::Extra(void) const
{
  return int16_t((data[e_h] << 8) | data[e_l]);
}

inline
uint8_t Sample::operator[](uint8_t i) const
{
  return data[i];
}

inline
uint8_t &Sample::operator[](uint8_t i)
{
  return data[i];
}
#endif

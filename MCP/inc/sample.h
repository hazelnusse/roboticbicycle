#ifndef SAMPLE_H
#define SAMPLE_H

#include <cstdint>

class Sample {
 public :
  // Magnetometer Measurements
  // Byte sized getters
  uint8_t magX_MSB(void) const;          // Getter
  uint8_t magX_LSB(void) const;          // Getter
  uint8_t magY_MSB(void) const;          // Getter
  uint8_t magY_LSB(void) const;          // Getter
  uint8_t magZ_MSB(void) const;          // Getter
  uint8_t magZ_LSB(void) const;          // Getter
  // 2-Byte sized getters
  int16_t magX(void) const;              // Getter
  int16_t magY(void) const;              // Getter
  int16_t magZ(void) const;              // Getter
  // Length 3 array int16_t getter
  void magXYZ(int16_t ar[3]) const;      // Getter
  // Byte sized setter
  void magX_MSB(const uint8_t val);      // Setter
  void magX_LSB(const uint8_t val);      // Setter
  void magY_MSB(const uint8_t val);      // Setter
  void magY_LSB(const uint8_t val);      // Setter
  void magZ_MSB(const uint8_t val);      // Setter
  void magZ_LSB(const uint8_t val);      // Setter
  // 2-Byte size setter
  void magX(const int16_t val);          // Setter
  void magY(const int16_t val);          // Setter
  void magZ(const int16_t val);          // Setter
  // Length 3 array int16_t setter
  void magXYZ(const int16_t ar[3]);      // Setter

  // Gyroscope Measurements
  // Byte sized getters
  uint8_t gyroT_MSB(void) const;         // Getter
  uint8_t gyroT_LSB(void) const;         // Getter
  uint8_t gyroX_MSB(void) const;         // Getter
  uint8_t gyroX_LSB(void) const;         // Getter
  uint8_t gyroY_MSB(void) const;         // Getter
  uint8_t gyroY_LSB(void) const;         // Getter
  uint8_t gyroZ_MSB(void) const;         // Getter
  uint8_t gyroZ_LSB(void) const;         // Getter
  // 2-Byte sized getters
  int16_t gyroT(void) const;             // Getter
  int16_t gyroX(void) const;             // Getter
  int16_t gyroY(void) const;             // Getter
  int16_t gyroZ(void) const;             // Getter
  // Length 4 array int16_t getter
  void gyroTXYZ(int16_t ar[4]) const;    // Getter
  // Byte sized setter
  void gyroT_MSB(const uint8_t val);     // Setter
  void gyroT_LSB(const uint8_t val);     // Setter
  void gyroX_MSB(const uint8_t val);     // Setter
  void gyroX_LSB(const uint8_t val);     // Setter
  void gyroY_MSB(const uint8_t val);     // Setter
  void gyroY_LSB(const uint8_t val);     // Setter
  void gyroZ_MSB(const uint8_t val);     // Setter
  void gyroZ_LSB(const uint8_t val);     // Setter
  // 2-Byte size setter
  void gyroT(const int16_t val);         // Setter
  void gyroX(const int16_t val);         // Setter
  void gyroY(const int16_t val);         // Setter
  void gyroZ(const int16_t val);         // Setter
  // Length 4 array int16_t setter
  void gyroTXYZ(const int16_t ar[4]);    // Setter

  // Accelerometer Measurements
  // Byte sized getters
  int8_t accT(void) const;               // Getter
  uint8_t accX_MSB(void) const;          // Getter
  uint8_t accX_LSB(void) const;          // Getter
  uint8_t accY_MSB(void) const;          // Getter
  uint8_t accY_LSB(void) const;          // Getter
  uint8_t accZ_MSB(void) const;          // Getter
  uint8_t accZ_LSB(void) const;          // Getter
  // 2-Byte sized getters
  int16_t accX(void) const;              // Getter
  int16_t accY(void) const;              // Getter
  int16_t accZ(void) const;              // Getter
  // Length 3 array int16_t getter
  int8_t accTXYZ(int16_t ar[3]) const;   // Getter
  // Byte sized setter
  void accT(const uint8_t val);          // Setter
  void accX_MSB(const uint8_t val);      // Setter
  void accX_LSB(const uint8_t val);      // Setter
  void accY_MSB(const uint8_t val);      // Setter
  void accY_LSB(const uint8_t val);      // Setter
  void accZ_MSB(const uint8_t val);      // Setter
  void accZ_LSB(const uint8_t val);      // Setter
  // 2-Byte size setter
  void accX(const int16_t val);          // Setter
  void accY(const int16_t val);          // Setter
  void accZ(const int16_t val);          // Setter
  // Length 4 array int16_t setter
  void accXYZ(const int16_t ar[3]);      // Setter

  // Byte size setter
  void Time_MSB(uint8_t val);             // Setter
  void Time_LSB(uint8_t val);             // Setter
  void Steer_MSB(uint8_t val);            // Setter
  void Steer_LSB(uint8_t val);            // Setter
  void RearWheel_MSB(uint8_t val);        // Setter
  void RearWheel_LSB(uint8_t val);        // Setter
  void FrontWheel_MSB(uint8_t val);       // Setter
  void FrontWheel_LSB(uint8_t val);       // Setter

  // 2-Byte size setters
  void Time(uint16_t val);                // Setter
  void Steer(uint16_t val);               // Setter
  void RearWheel(uint16_t val);           // Setter
  void FrontWheel(uint16_t val);          // Setter

  // Byte size getters
  uint8_t Time_MSB(void) const;            // Getter
  uint8_t Time_LSB(void) const;            // Getter
  uint8_t Steer_MSB(void) const;           // Getter
  uint8_t Steer_LSB(void) const;           // Getter
  uint8_t RearWheel_MSB(void) const;       // Getter
  uint8_t RearWheel_LSB(void) const;       // Getter
  uint8_t FrontWheel_MSB(void) const;      // Getter
  uint8_t FrontWheel_LSB(void) const;      // Getter

  // 2-Byte size getters
  uint16_t Time(void) const;               // Getter
  uint16_t Steer(void) const;              // Getter
  uint16_t RearWheel(void) const;          // Getter
  uint16_t FrontWheel(void) const;         // Getter

  // Overload operator[]
  uint8_t operator[](uint32_t index) const;
  uint8_t &operator[](uint32_t index);

 private :
  uint8_t data[29];
};

inline
uint8_t Sample::magX_MSB(void) const
{
  return data[0];
}

inline
uint8_t Sample::magX_LSB(void) const
{
  return data[1];
}

inline
uint8_t Sample::magY_MSB(void) const
{
  return data[2];
}

inline
uint8_t Sample::magY_LSB(void) const
{
  return data[3];
}

inline
uint8_t Sample::magZ_MSB(void) const
{
  return data[4];
}

inline
uint8_t Sample::magZ_LSB(void) const
{
  return data[5];
}

inline
int16_t Sample::magX(void) const
{
  return int16_t((magX_MSB() << 8) | magX_LSB());
}

inline
int16_t Sample::magY(void) const
{
  return int16_t((magY_MSB() << 8) | magY_LSB());
}

inline
int16_t Sample::magZ(void) const
{
  return int16_t((magZ_MSB() << 8) | magZ_LSB());
}

inline
void Sample::magXYZ(int16_t ar[3]) const
{
  ar[0] = magX();
  ar[1] = magY();
  ar[2] = magZ();
}

// Magnetometer Setters
inline
void Sample::magX_MSB(const uint8_t val)
{
  data[0] = val;
}

inline
void Sample::magX_LSB(const uint8_t val)
{
  data[1] = val;
}

inline
void Sample::magY_MSB(const uint8_t val)
{
  data[2] = val;
}

inline
void Sample::magY_LSB(const uint8_t val)
{
  data[3] = val;
}

inline
void Sample::magZ_MSB(const uint8_t val)
{
  data[4] = val;
}

inline
void Sample::magZ_LSB(const uint8_t val)
{
  data[5] = val;
}

inline
void Sample::magX(const int16_t val)
{
  magX_MSB(uint8_t(val >> 8));
  magX_LSB(uint8_t(val));
}

inline
void Sample::magY(const int16_t val)
{
  magY_MSB(uint8_t(val >> 8));
  magY_LSB(uint8_t(val));
}

inline
void Sample::magZ(const int16_t val)
{
  magZ_MSB(uint8_t(val >> 8));
  magZ_LSB(uint8_t(val));
}

inline
void Sample::magXYZ(const int16_t ar[3])
{
  magX(ar[0]);
  magY(ar[1]);
  magZ(ar[2]);
}

// Gyroscope Getters
inline
uint8_t Sample::gyroT_MSB(void) const
{
  return data[6];
}

inline
uint8_t Sample::gyroT_LSB(void) const
{
  return data[7];
}

inline
uint8_t Sample::gyroX_MSB(void) const
{
  return data[8];
}

inline
uint8_t Sample::gyroX_LSB(void) const
{
  return data[9];
}

inline
uint8_t Sample::gyroY_MSB(void) const
{
  return data[10];
}

inline
uint8_t Sample::gyroY_LSB(void) const
{
  return data[11];
}

inline
uint8_t Sample::gyroZ_MSB(void) const
{
  return data[12];
}

inline
uint8_t Sample::gyroZ_LSB(void) const
{
  return data[13];
}

inline
int16_t Sample::gyroT(void) const
{
  return int16_t((gyroT_MSB() << 8) | gyroT_LSB());
}

inline
int16_t Sample::gyroX(void) const
{
  return int16_t((gyroX_MSB() << 8) | gyroX_LSB());
}

inline
int16_t Sample::gyroY(void) const
{
  return int16_t((gyroY_MSB() << 8) | gyroY_LSB());
}

inline
int16_t Sample::gyroZ(void) const
{
  return int16_t((gyroZ_MSB() << 8) | gyroZ_LSB());
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
void Sample::gyroT_MSB(const uint8_t val)
{
  data[6] = val;
}

inline
void Sample::gyroT_LSB(const uint8_t val)
{
  data[7] = val;
}

inline
void Sample::gyroX_MSB(const uint8_t val)
{
  data[8] = val;
}

inline
void Sample::gyroX_LSB(const uint8_t val)
{
  data[9] = val;
}

inline
void Sample::gyroY_MSB(const uint8_t val)
{
  data[10] = val;
}

inline
void Sample::gyroY_LSB(const uint8_t val)
{
  data[11] = val;
}

inline
void Sample::gyroZ_MSB(const uint8_t val)
{
  data[12] = val;
}

inline
void Sample::gyroZ_LSB(const uint8_t val)
{
  data[13] = val;
}

inline
void Sample::gyroT(const int16_t val)
{
  gyroT_MSB(uint8_t(val >> 8));
  gyroT_LSB(uint8_t(val));
}

inline
void Sample::gyroX(const int16_t val)
{
  gyroX_MSB(uint8_t(val >> 8));
  gyroX_LSB(uint8_t(val));
}

inline
void Sample::gyroY(const int16_t val)
{
  gyroY_MSB(uint8_t(val >> 8));
  gyroY_LSB(uint8_t(val));
}

inline
void Sample::gyroZ(const int16_t val)
{
  gyroZ_MSB(uint8_t(val >> 8));
  gyroZ_LSB(uint8_t(val));
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
int8_t Sample::accT(void) const
{
  return int8_t(data[14]);
}

inline
uint8_t Sample::accX_MSB(void) const
{
  return data[15];
}

inline
uint8_t Sample::accX_LSB(void) const
{
  return data[16];
}

inline
uint8_t Sample::accY_MSB(void) const
{
  return data[17];
}

inline
uint8_t Sample::accY_LSB(void) const
{
  return data[18];
}

inline
uint8_t Sample::accZ_MSB(void) const
{
  return data[19];
}

inline
uint8_t Sample::accZ_LSB(void) const
{
  return data[20];
}

inline
int16_t Sample::accX(void) const
{
  return int16_t((accX_MSB() << 8) | accX_LSB());
}

inline
int16_t Sample::accY(void) const
{
  return int16_t((accY_MSB() << 8) | accY_LSB());
}

inline
int16_t Sample::accZ(void) const
{
  return int16_t((accZ_MSB() << 8) | accZ_LSB());
}

inline
int8_t Sample::accTXYZ(int16_t ar[3]) const
{
  ar[0] = accX();
  ar[1] = accY();
  ar[2] = accZ();
  return accT();
}

// Accelerometer Setters
inline
void Sample::accT(const uint8_t val)
{
  data[14] = val;
}

inline
void Sample::accX_MSB(const uint8_t val)
{
  data[15] = val;
}

inline
void Sample::accX_LSB(const uint8_t val)
{
  data[16] = val;
}

inline
void Sample::accY_MSB(const uint8_t val)
{
  data[17] = val;
}

inline
void Sample::accY_LSB(const uint8_t val)
{
  data[18] = val;
}

inline
void Sample::accZ_MSB(const uint8_t val)
{
  data[19] = val;
}

inline
void Sample::accZ_LSB(const uint8_t val)
{
  data[20] = val;
}

inline
void Sample::accX(const int16_t val)
{
  accX_MSB(uint8_t(val >> 8));
  accX_LSB(uint8_t(val));
}

inline
void Sample::accY(const int16_t val)
{
  accY_MSB(uint8_t(val >> 8));
  accY_LSB(uint8_t(val));
}

inline
void Sample::accZ(const int16_t val)
{
  accZ_MSB(uint8_t(val >> 8));
  accZ_LSB(uint8_t(val));
}

inline
void Sample::accXYZ(const int16_t ar[3])
{
  accX(ar[0]);
  accY(ar[1]);
  accZ(ar[2]);
}

inline
void Sample::Time_MSB(uint8_t val)
{
  data[21] = val;
}

inline
void Sample::Time_LSB(uint8_t val)
{
  data[22] = val;
}

inline
void Sample::Steer_MSB(uint8_t val)
{
  data[23] = val;
}

inline
void Sample::Steer_LSB(uint8_t val)
{
  data[24] = val;
}

inline
void Sample::RearWheel_MSB(uint8_t val)
{
  data[25] = val;
}

inline
void Sample::RearWheel_LSB(uint8_t val)
{
  data[26] = val;
}

void Sample::FrontWheel_MSB(uint8_t val)
{
  data[27] = val;
}

void Sample::FrontWheel_LSB(uint8_t val)
{
  data[28] = val;
}

inline
void Sample::Time(uint16_t val)
{
  Time_MSB((uint8_t) (val >> 8));
  Time_LSB((uint8_t) val);
}

inline
void Sample::Steer(uint16_t val)
{
  Steer_MSB((uint8_t) (val >> 8));
  Steer_LSB((uint8_t) val);
}

inline
void Sample::RearWheel(uint16_t val)
{
  RearWheel_MSB((uint8_t) (val >> 8));
  RearWheel_LSB((uint8_t) val);
}

inline
void Sample::FrontWheel(uint16_t val)
{
  FrontWheel_MSB((uint8_t) (val >> 8));
  FrontWheel_LSB((uint8_t) val);
}

inline
uint8_t Sample::Time_MSB(void) const
{
  return data[21];
}

inline
uint8_t Sample::Time_LSB(void) const
{
  return data[22];
}

inline
uint8_t Sample::Steer_MSB(void) const
{
  return data[23];
}

inline
uint8_t Sample::Steer_LSB(void) const
{
  return data[24];
}

inline
uint8_t Sample::RearWheel_MSB(void) const
{
  return data[25];
}

inline
uint8_t Sample::RearWheel_LSB(void) const
{
  return data[26];
}

inline
uint8_t Sample::FrontWheel_MSB(void) const
{
  return data[27];
}

inline
uint8_t Sample::FrontWheel_LSB(void) const
{
  return data[28];
}

inline
uint16_t Sample::Time(void) const
{
  return (uint16_t) ((Time_MSB() << 8) | Time_LSB());
}

inline
uint16_t Sample::Steer(void) const
{
  return (uint16_t) ((Steer_MSB() << 8) | Steer_LSB());
}

inline
uint16_t Sample::RearWheel(void) const
{
  return (uint16_t) ((RearWheel_MSB() << 8) | RearWheel_LSB());
}

inline
uint16_t Sample::FrontWheel(void) const
{
  return (uint16_t) ((FrontWheel_MSB() << 8) | FrontWheel_LSB());
}

inline
uint8_t Sample::operator[](uint32_t index) const
{
  return data[index];
}

inline
uint8_t &Sample::operator[](uint32_t index)
{
  return data[index];
}

#endif

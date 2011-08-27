#include "sample.h"

// Raw measurements are represented as uint8_t array with 29 elements in the
// following order:
//
//  0 -- Magnetometer X MSB
//  1 -- Magnetometer X LSB
//  2 -- Magnetometer Y MSB
//  3 -- Magnetometer Y LSB
//  4 -- Magnetometer Z MSB
//  5 -- Magnetometer Z LSB
//  6 -- Gyroscope Temperature MSB
//  7 -- Gyroscope Temperature LSB
//  8 -- Gyroscope X MSB
//  9 -- Gyroscope X LSB
// 10 -- Gyroscope Y MSB
// 11 -- Gyroscope Y LSB
// 12 -- Gyroscope Z MSB
// 13 -- Gyroscope Z LSB
// 14 -- Accelerometer Temp
// 15 -- Accelerometer X MSB
// 16 -- Accelerometer X LSB
// 17 -- Accelerometer Y MSB
// 18 -- Accelerometer Y LSB
// 19 -- Accelerometer Z MSB
// 20 -- Accelerometer Z LSB
// 21 -- Timestamp MSB
// 22 -- Timestamp LSB
// 23 -- Steer MSB
// 24 -- Steer LSB
// 25 -- Rear wheel MSB
// 26 -- Rear wheel LSB
// 27 -- Front wheel MSB
// 28 -- Front wheel LSB


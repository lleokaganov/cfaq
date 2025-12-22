#ifndef I2C_LIB_H
#define I2C_LIB_H

#include <Wire.h>
#include "driver/i2c.h"

// bool i2c_ping_fast(uint8_t addr, uint8_t len, uint32_t timeout_ms);
// // bool i2c_ping_fast(uint8_t addr, uint32_t timeout_ms);

#define I2C_BUF_MAX_SIZE 128

uint8_t I2C_write(uint8_t dev, uint8_t reg, String s);
bool I2C_read(uint8_t dev, uint8_t reg, uint8_t nBytes, uint8_t* buffer);
bool I2C_read_raw(uint8_t dev, uint8_t nBytes, uint8_t* buffer);

extern uint32_t I2C_timeout_ms;
extern uint8_t I2C_pin_SDA;  // default SDA pin
extern uint8_t I2C_pin_SCL;  // default SCL pin

#endif
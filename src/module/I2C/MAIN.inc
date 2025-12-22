#include "module/I2C/h.hpp"

uint32_t I2C_timeout_ms = 50;  // default timeout
uint8_t I2C_pin_SDA = 0xFF;  // default SDA pin
uint8_t I2C_pin_SCL = 0xFF;  // default SCL pin

// Запись в регистр I2C-устройства
uint8_t I2C_write(uint8_t dev, uint8_t reg, String s) {
    int count = 1; for(int i=0; i < s.length(); i++) { if(s[i] == ' ') count++; }
    Wire.beginTransmission(dev);
    Wire.write(reg);
    for (int i = 0; i < count; i++) {
        uint8_t val = PARG0(s, i);
        Wire.write(val);
    }
    // 0: success.
    // 1: data too long to fit in transmit buffer.
    // 2: received NACK on transmit of address.
    // 3: received NACK on transmit of data.
    // 4: other error.
    // 5: timeout
    return Wire.endTransmission();
}

bool I2C_read(uint8_t dev, uint8_t reg, uint8_t nBytes, uint8_t* buffer) {
    Serial.printf("I2C_read: dev=0x%02X reg=0x%02X nBytes=%d\n", dev, reg, nBytes); // DEBUG
    if(nBytes > I2C_BUF_MAX_SIZE) return false;
    if(reg != 0xFF) {
        Wire.beginTransmission(dev);
        Wire.write(reg);
        Wire.endTransmission(false);
    }
    int received = Wire.requestFrom((uint8_t)dev, (uint8_t)nBytes, (uint8_t)1);
    Serial.printf("   received %d bytes\n", received); // DEBUG
    if (received != nBytes) {
        Serial.printf("ERROR I2C_read: requested %d bytes, but received %d bytes\n", nBytes, received);
        return false;
    }

    for (int i = 0; i < nBytes; i++) {
        if(!Wire.available()) {
            Serial.printf("ERROR I2C_read: !available\n");
            return false;
        }
        buffer[i] = Wire.read();
    }
    return true;
}

// bool i2c_ping_fast(uint8_t addr, uint32_t timeout_ms = 5) {
//     Serial.printf("i2c_ping_fast: addr=0x%02X timeout_ms=%d\n", addr, timeout_ms); // DEBUG
//     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//     i2c_master_start(cmd);
//     i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, true);
//     i2c_master_stop(cmd);
//     esp_err_t res = i2c_master_cmd_begin(I2C_NUM_0, cmd, timeout_ms / portTICK_PERIOD_MS);
//     i2c_cmd_link_delete(cmd);
//     return res == ESP_OK;
// }
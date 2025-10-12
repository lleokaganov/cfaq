#include <Wire.h>
#include <Adafruit_PN532.h>

#ifdef ESP32
    TwoWire wire0(0);  // I2C шина 0
    TwoWire wire1(1);  // I2C шина 1
#endif
    Adafruit_PN532* PN532_nfc = nullptr;

    uint16_t PN532_timeout = 50;
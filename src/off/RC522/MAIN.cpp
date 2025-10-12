#define USE_SPI 1

// #include <SPI.h>

// wiring the MFRC522 to ESP8266 (ESP-12)
// RST     = GPIO5
// SDA(SS) = GPIO4
// MOSI    = GPIO13
// MISO    = GPIO12
// SCK     = GPIO14
// GND     = GND
// 3.3V    = 3.3V
#define RST_PIN 5
#define SS_PIN 0 // 4

#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);

byte NFC_init = 0;

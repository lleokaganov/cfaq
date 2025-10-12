#ifndef _CONFIG_H
#define _CONFIG_H

#include <Arduino.h>

#define NLOOP 16  // максимум циклов loopes
#define NSET 50  // Максимальное количество переменных

#define DEFAULTPORT 80 // отвечающий порт вебки
#define USE_WEB  // использовать веб-интерфейс

#define WIFIudp_localport 8888  // порт для служебного UDP

#define DEFAULT_NBUF 64 // размер буфера для пинов, должен быть степенью двойки

#define WEB_START_DEFAULT 1
#define SHEDULE_START_DEFAULT 1

#define LOGISET_DEFAULT 0xFFFF // по умолчанию логи все
// #define CODE_MINIMUM

// #define MP3
// #define USE_WIEGAND
// #define USE_RC522
// #define USE_SPI
// #define USE_MQTT
// #define USE_DB18B20
// #define USE_UART
// #define USE_ENCODER

// #define USE_SPI
#define USE_TFT_SPI

// #define USE_NFC_RF430
// #define USE_NFC_PN532
// #define USE_RFID2

#define USE_LED_WS

// #define USE_CAMERA
// #define USE_TFT_LGFX
// #define DisplayDriver Panel_GC9A01	//	lgfx::Panel_GC9A01	240x240 (круглый)
// #define DisplayDriver Panel_ST7789	//	lgfx::Panel_ST7789	240x240 / 320x240
// #define DisplayDriver Panel_ILI9341	//	lgfx::Panel_ILI9341	320x240
// #define DisplayDriver Panel_ILI9488	//	lgfx::Panel_ILI9488	480x320
// #define DisplayDriver Panel_GC9107	//	lgfx::Panel_GC9107	240x280
// #define DisplayDriver Panel_SSD1351	// 	(OLED)	lgfx::Panel_SSD1351	128x128 / 128x96
// #define DisplayDriver Panel_ST7735	//	lgfx::Panel_ST7735	128x160
// #define DisplayDriver Panel_HX8357	//	lgfx::Panel_HX8357	480x320


#endif // _CONFIG_H
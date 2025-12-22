#pragma once

#define NLOOP 16  // максимум циклов loopes
#define NSET 50  // Максимальное количество переменных
#define ECHOMOTO_MAX 4000 // Максимальный размер буфера EchoMOTO

#define DEFAULTPORT 80 // отвечающий порт вебки
#define USE_WEB  // использовать веб-интерфейс

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
// #define USE_UART
// #define USE_ENCODER
// #define USE_UDP

// #define USE_SPI
// #define USE_TFT_SPI

#define USE_I2C
// #define USE_LCD_I2C

#define USE_ONEWIRE
#define USE_DS18B20 // Далласовский термоментр DS18B20 самый популярный, требует USE_ONEWIRE
#define USE_KY_IIC_3V3 // Датчик давления до 10 Bar и температуры, здоровенная китайская гайка, требует USE_I2C

// #define USE_NFC_RF430
// #define USE_NFC_PN532
// #define USE_RFID2

// #define USE_LED_WS

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

#define USE_TFT_ST7796
#define USE_QR_CODE

#define USE_WS_CLIENT
#define USE_UPGRADE_OTA

// #define USE_HTTPS

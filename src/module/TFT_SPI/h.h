#ifndef LED_TFT_SPI_H
#define LED_TFT_SPI_H

//---------------- TFT SPI

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "sys/ARG.h"

// Создаём указатель на дисплей
extern Adafruit_ST7789* tft;

extern uint16_t last_tft_color;
extern uint16_t get_tft_color0(String s, uint16_t n);
extern uint16_t get_tft_color(String s, uint16_t n);

// #include <SPI.h>

#endif // LED_TFT_SPI_H
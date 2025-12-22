#pragma once
#include <Arduino.h>
#include "main.h"

// --- Аппаратно-зависимые заголовки ---
#ifdef ESP32
  #include "esp_wifi.h"
  extern "C" uint8_t temprature_sens_read(void);
#endif

#ifdef ESP32
 #ifdef __cplusplus
  extern "C" {
 #endif
  uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();
#endif

// --- Внешние зависимости (другие модули) ---

// --- Прототипы функций ---

// Основные
String REPER(String s);
String REPER_complex(String l);
String REPER_simplex(String l);
String REPERFILE(String url);

// Доп. утилиты
int dBm2percent(int d);
String WIFI_all_txt(int n);

// --- Внешние переменные, объявленные в других модулях ---
extern String EchoMOTO;

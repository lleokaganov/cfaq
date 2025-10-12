#ifndef REPER_H
#define REPER_H

#include <Arduino.h>

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
#include "sys/file.h"
#include "sys/LOG.h"
#include "config.h"
#include "main.h"

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
// extern uint32_t UnixTime;
// extern uint16_t NBUF;
// extern uint8_t NLOOP;
// extern uint8_t NSET;

// Константы и версии (если определяются в main.cpp)
// extern const char* VERNAME;
// extern uint32_t VERID;

// // Вспомогательные структуры (если нужны)
// struct LoopesStruct {
//   uint32_t count;
//   uint32_t value;
//   uint8_t one;
//   String file;
// };

// struct SettesStruct {
//   String n;
//   String v;
// };

// extern LoopesStruct Loopes[];
// extern SettesStruct Settes[];

#endif // REPER_H

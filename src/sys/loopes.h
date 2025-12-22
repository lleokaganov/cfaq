#pragma once
#include <Arduino.h>

// ====================================================
// Прототипы функций управления циклами
// ====================================================

// extern LoopStruct Loopes[NLOOP];

void loop_del(uint8_t cikl);
uint8_t loop_new(uint16_t one, uint8_t cikl, uint32_t value, uint32_t count, String file);
uint8_t loop_new(uint16_t one, uint8_t cikl, uint32_t value, uint32_t count, String file, byte yild);
uint8_t loop_now(String file);

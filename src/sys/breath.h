#ifndef BREATH_H
#define BREATH_H

#include <Arduino.h>

struct BreathStruct {
  uint32_t TT;
  uint32_t TTT;
  uint8_t pinok;
  uint16_t Z;
  uint16_t min;
  uint16_t max;
  int16_t K;
  uint32_t CY;
};
extern BreathStruct Breath;

void breath(void);

#endif

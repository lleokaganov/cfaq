/*
#include "ENCODER.h"

// ---- ENCODER ----
volatile int ENCODER = 0; // счётчик
volatile byte ENCODER_last = 0, ENCODER_A=13, ENCODER_B=14;
const int8_t ENCODER_tab[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};

void ICACHE_RAM_ATTR ENCODER_INT() {
  byte x = digitalRead(ENCODER_A) | digitalRead(ENCODER_B) << 1;
  ENCODER += ENCODER_tab[ x | (ENCODER_last << 2) ];
  ENCODER_last = x;
  // byte p=CF("sound.pin").toInt(); digitalWrite(p,!digitalRead(p));
}

// void ICACHE_RAM_ATTR ENCODER_INT() {
//  byte x = (GPIO_REG_READ(GPIO_IN_ADDRESS) >> ENCODER_A & 1) | ((GPIO_REG_READ(GPIO_IN_ADDRESS) >> ENCODER_B & 1) << 1);
//  ENCODER += ENCODER_tab[x | (ENCODER_last << 2)];
//  ENCODER_last = x;
//}

int ENCODER_old=ENCODER;
*/
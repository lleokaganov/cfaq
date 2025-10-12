#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

#include "config.h"
#include "loopes.h"
#include "FLT.h"

#include <Ticker.h>  //Ticker Library
#include <time.h> // дату чтобы конвертировать из UnixTime

void start_timer(); // запуск таймера
void pinlog_engine(); // процедура таймера-следилки за пинами

#define KBIT0  0b0000000000100000
#define KBIT1  0b0000000001000000
#define KBIT2  0b0000000010000000
#define KBIT3  0b0000000100000000
#define KBIT4  0b0000001000000000
#define KBIT5  0b0000010000000000
#define KBIT12 0b0000100000000000
#define KBIT13 0b0001000000000000
#define KBIT14 0b0010000000000000
#define KBIT15 0b0100000000000000
#define KBIT16 0b1000000000000000

extern Ticker timpin;

extern unsigned long UnixTime; // текущее время по Unix
extern uint16_t NBUF; // максимальный размер буфера должен быть строго кратен степени двойки, напр. 64, 128, 256, 512

extern struct BufLoopes { // шаблон таймера
  uint16_t kn;
  uint16_t A0;
} *Buf;
extern byte bufn;

#endif // TIMER_H
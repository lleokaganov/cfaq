#include <Arduino.h>
#include "main.h"

uint64_t UnixTime = 0; // текущее время по Unix

void tim_engine() { // процедура ежесекундного таймера
  UnixTime++;
  for (uint8_t i = 0; i < NLOOP; i++) {
    if (Loopes[i].value == 0) continue; // нет такого цикла
    if (Loopes[i].count >= Loopes[i].value) continue; // уже пошёл на выполнение
    Loopes[i].count++;
  }
}

Ticker tim;

void start_timer() {
  tim.attach(1, tim_engine); // таймер на 1 sec
}

uint16_t NBUF = DEFAULT_NBUF; // максимальный размер буфера должен быть строго кратен степени двойки, напр. 64, 128, 256, 512

// BufLoopes *Buf;
// BufLoopes Buf[DEFAULT_NBUF];
// extern uint16_t NBUF;

byte bufn = 0;
BufLoopes *Buf = NULL;

Ticker timpin;

void pinlog_engine() { // процедура таймера-следилки за пинами

  uint16_t c;
  if(CF("MEASURE_ENGINE") != "") {
    MOTO(CF("MEASURE_ENGINE"), 0);
    c = CF0("MEASURE_RESULT");
  } else {
    #ifdef ESP32
      c = analogRead(32);
    #else
      c = analogRead(A0);
    #endif
  }

  byte flt = FLT(c);
  if(Buf!=NULL) {
    bufn = (++bufn) & (NBUF - 1);
    Buf[bufn].A0 = c;
    Buf[bufn].kn = flt
        + (digitalRead(0) ? KBIT0 : 0)
        + (digitalRead(1) ? KBIT1 : 0)
        + (digitalRead(2) ? KBIT2 : 0)
        + (digitalRead(3) ? KBIT3 : 0)
        + (digitalRead(4) ? KBIT4 : 0)
        + (digitalRead(5) ? KBIT5 : 0)
        + (digitalRead(12) ? KBIT12 : 0)
        + (digitalRead(13) ? KBIT13 : 0)
        + (digitalRead(14) ? KBIT14 : 0)
        + (digitalRead(15) ? KBIT15 : 0)
        + (digitalRead(16) ? KBIT16 : 0);
  }
#ifdef USE_FLT
  // if ( c >= MaxA0 ) { MOTO(CF("MaxA0.callback"), 0); }
  // else if ( c < MinA0 ) { MOTO(CF("MinA0.callback"), 0); }
  // if ( flt == FltA0 ) { MOTO(CF("FltA0.callback"), 0); }
#endif
}

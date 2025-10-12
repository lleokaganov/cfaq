#include <Arduino.h>
#include "loopes.h"

void loop_del(uint8_t cikl) {
  Loopes[cikl].value = 0;
}

uint8_t loop_new(uint16_t one, uint8_t cikl, uint32_t value, uint32_t count, String file) {
  return loop_new(one, cikl, value, count, file, 1);
}
uint8_t loop_new(uint16_t one, uint8_t cikl, uint32_t value, uint32_t count, String file, byte yild) { // set_new_LOOP()
  if (cikl != 255 && cikl > NLOOP) {
    ERR("LOOP: wrong cikl");
    return 255;
  }
  if (cikl == 255) { // номер цикла предстоит найти
    for (uint8_t i = 0; i < NLOOP; i++) {
      if (Loopes[i].file == "" && cikl != 255 ) break; // найден первый же пустой цикл, не крутить
      if (cikl == 255 && Loopes[i].value == 0) cikl = i; // поиск свободного места
    } if (cikl == 255) {
      ERR("LOOP: no room");
      return 255;
    }
  }
  Loopes[cikl].count = 0;
  Loopes[cikl].value = value;
  Loopes[cikl].one = one;
  Loopes[cikl].file = file;
  Loopes[cikl].yild = yild;
  LOGI(LOG_MAIN, "New loop [" + String(cikl) + "]: one=" + String(one) + " value=" + String(value) + " count=" + String(count) + " yild=" + String(yild) + " {" + file + "}");
  return cikl;
}

uint8_t loop_now(String file) { // set_NOW_LOOP()
   uint8_t i = 0;
   while(i < NLOOP && Loopes[i].value != 0 ) i++;
   if(i == NLOOP-1 && Loopes[i].value != 0) { ERR("LOOP: no room"); return 255; }
   Loopes[i].count = 1;
   Loopes[i].value = 1;
   Loopes[i].one = 1;
   Loopes[i].file = file;
   Loopes[i].yild = 1;
   // LOGI(LOG_MAIN, "New loop [" + String(cikl) + "]: one=" + String(one) + " value=" + String(value) + " count=" + String(count) + " yild=" + String(yild) + " {" + file + "}");
   return i;
}

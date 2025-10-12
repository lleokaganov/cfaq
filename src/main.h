#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

#define VERID "5.57"
#define VERNAME "HOBOT5"
#define HELP_NET_NAME "ESP-HELP"

#include "config.h"
#include "sys/LOG.h"
#include "sys/timer.h"
#include "sys/breath.h"
#include "sys/ARG.h"
#include "sys/MOTO.h"
#include "sys/REPER.h"

#ifdef USE_WEB
  #include "sys/WEB.h"
#endif

#ifdef ESP32
     #define RESET_FORCE_PIN 16
     #define RESET_FORCE_PIN_SET 0
#else
     #define RESET_FORCE_PIN 16
     #define RESET_FORCE_PIN_SET 0
#endif

// #include "templates.h"


// #include "sys/upgrade.h"
// #include "sys/file.h"
// #include "sys/play.h"

// #include "sys/mainutils.h"
// #include "sys/LOG.h"
// #include "sys/wifi.h"
// // #include "sys/FS.h"
// #include "sys/FLT.cpp"

extern String EchoMOTO; // строка для вывода инфы в Аякс
extern byte SerialSwap; // состояние переключения Serial

extern byte webstart; // Обслуживать ли веб-сервер
extern byte SheduleInit; // Планировщик запущен

// LOOPES

// struct StructLoopes { // шаблон таймера
//   uint32_t count = 0;
//   uint32_t value = 0;
//   uint16_t one = 0; // число повторов (0 - повторять вечно)
//   String file = "";
//   byte yild = 0; // приоритет: 1 - без приоритета
// };
// StructLoopes Loopes[NLOOP]; // volatile не работает почему-то, может и не надо

struct LoopStruct {
  uint16_t one;     // период
  uint8_t yild;     // разрешение yield()
  uint32_t value;   // активность
  uint32_t count;   // счётчик итераций
  String file;      // файл/команда для выполнения
};
extern LoopStruct Loopes[NLOOP];

// SETTES

// // Структура для хранения пары "имя-значение"
// struct StructSettes {
//   String n = "";  // Имя переменной
//   String v = "";  // Значение переменной
// };
// // Глобальный массив настроек
// StructSettes Settes[NSET];

// Структура для хранения пар ключ-значение
struct Setting {
    String n;  // Имя настройки
    String v;  // Значение настройки
};
// Глобальный массив настроек
extern Setting Settes[NSET];






// === Interrupts ===
// extern volatile uint32_t InterruptsFlags;
// extern byte interrupts_tab[];

void ICACHE_RAM_ATTR INTERRUPT_0();
void ICACHE_RAM_ATTR INTERRUPT_1();
void ICACHE_RAM_ATTR INTERRUPT_2();
void ICACHE_RAM_ATTR INTERRUPT_3();
void ICACHE_RAM_ATTR INTERRUPT_4();
void ICACHE_RAM_ATTR INTERRUPT_5();
void ICACHE_RAM_ATTR INTERRUPT_6();
void ICACHE_RAM_ATTR INTERRUPT_7();
void ICACHE_RAM_ATTR INTERRUPT_8();
void ICACHE_RAM_ATTR INTERRUPT_9();

#ifdef ESP32
void ICACHE_RAM_ATTR INTERRUPT_10();
void ICACHE_RAM_ATTR INTERRUPT_11();
void ICACHE_RAM_ATTR INTERRUPT_12();
void ICACHE_RAM_ATTR INTERRUPT_13();
void ICACHE_RAM_ATTR INTERRUPT_14();
void ICACHE_RAM_ATTR INTERRUPT_15();
void ICACHE_RAM_ATTR INTERRUPT_16();
void ICACHE_RAM_ATTR INTERRUPT_17();
void ICACHE_RAM_ATTR INTERRUPT_18();
void ICACHE_RAM_ATTR INTERRUPT_19();
void ICACHE_RAM_ATTR INTERRUPT_20();
void ICACHE_RAM_ATTR INTERRUPT_21();
void ICACHE_RAM_ATTR INTERRUPT_22();
void ICACHE_RAM_ATTR INTERRUPT_23();
void ICACHE_RAM_ATTR INTERRUPT_24();
void ICACHE_RAM_ATTR INTERRUPT_25();
#endif

















#endif // MAIN_H
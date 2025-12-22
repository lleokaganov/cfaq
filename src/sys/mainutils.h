#pragma
#include <Arduino.h>
#include <MD5Builder.h>

// ============================================================
// Прототипы функций из mainutils.cpp
// ============================================================

// --- Строковые утилиты ---
String MD5(String s);
String njsn(String s);
String njs(String s);
String h(String s);
String urlencode(String &s);

// --- Инфо и вывод ---
String FullInfo();
void SYSP(String &s);
void iecho(String s);

String HH(uint8_t x); // в 16-ричное число
String HH0X(uint8_t x); // в 16-ричное число с префиксом 0x

// --- Время и форматирование ---
String D00(byte x);
String Time_hh(uint32_t T);
String Time_mm(uint32_t T);
String Time_ss(uint32_t T);
String Time_dn(uint32_t T);
String Time_ddn(uint32_t T);
String Time_hhmmss(uint32_t T);

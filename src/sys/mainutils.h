#ifndef MAINUTILS_H
#define MAINUTILS_H

#include <Arduino.h>
#include "DOMOTO.h"
#include "templates.h"
#include "LOG.h"
#include "file.h"

#include <MD5Builder.h>

// ============================================================
// Прототипы функций из mainutils.cpp
// ============================================================

// --- Строковые утилиты ---
String MD5(String s);
String njsn(String s);
String njs(String s);
String h(String s);
String urlencode(String s);

// --- Инфо и вывод ---
String FullInfo();
void SYSP(String s);
void iecho(String s);

// --- Время и форматирование ---
String D00(byte x);
String Time_hh(uint32_t T);
String Time_mm(uint32_t T);
String Time_ss(uint32_t T);
String Time_dn(uint32_t T);
String Time_ddn(uint32_t T);
String Time_hhmmss(uint32_t T);

#endif // MAINUTILS_H

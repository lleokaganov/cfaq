#pragma once
#include <Arduino.h>

// Количество типов логов
#define LOGNAMES_N 9

// Названия логов
const String LOGNAMES[LOGNAMES_N] = {
    "MAIN", "ERROR", "WEB", "WGET", "UPGRADE", "MP3", "IECHO", "PROG", "CONN"
};

// Константы для типов логов
#define LOG_MAIN 0       // Системные логи
#define LOG_ERROR 1      // Ошибки
#define LOG_WEB 2        // Веб-запросы
#define LOG_WGET 3       // Запросы данных к внешним серверам
#define LOG_UPGRADE 4    // Логи апгрейда
#define LOG_MP3 5        // MP3 логи
#define LOG_IECHO 6      // Логи IECHO
#define LOG_PROG 7       // Логи выполнения MOTO
#define LOG_CONN 8       // Логи сервисов (MQTT, MODBUS)

// Установка логов по умолчанию
extern uint16_t LOGISET;

// Прототипы функций
String LOG_I_NAME(byte i);
byte LOG_NAME_I(String s);

void LOGI(byte i, String s, String music);
void LOGI(byte i, String s);
void LOGI(byte i, String s, byte noN);
// void LOGI(uint8_t level, const String &msg);
// void ERR(const String &msg);

bool LOGLI(byte i);

String ERR(String s);
String ERR(String s, String o);

String LOG(String s);
String LOG(String s, byte noN);

#define LOG_MAIN 0

#ifndef UDP_H
#define UDP_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include "LOG.h"
#include "MOTO.h"
#include "upgrade.h"
#include "config.h"

// // #include "wifi.h"
// #include "mainutils.h"
// #include "upgrade.h"
// #include "main.h"

// ============================================================
// Константы
// ============================================================

#define WIFIudp_localport 8888  // порт для служебного UDP

// ============================================================
// Глобальные переменные (объявлены extern, определяются в udp.cpp)
// ============================================================

extern WiFiUDP WIFIudp;          // основной UDP для системных задач (NTP, пинг и т.д.)
extern int8_t WIFIudp_flag;      // <0 — не инициализирован, 0 — свободен, 1/123/124 — занят операцией
extern uint32_t WIFIudp_timestamp; // момент последнего обмена

extern WiFiUDP MERAudp;          // отдельный UDP для “одноразовых” отправок (без приёма)
extern byte MERA_status;         // служебный статус

// ============================================================
// Прототипы функций
// ============================================================

/**
 * @brief Обработчик входящих UDP-пакетов.
 * 
 * В зависимости от WIFIudp_flag:
 *  - 123 → NTP время
 *  - 124 → UDP ответ для MOTO
 *  - 125 → UDP ответ для Upgrade
 *  - иначе → сохраняет результат в CF("UDP_result")
 */
void WIFIudp_result(void);

#endif // UDP_H

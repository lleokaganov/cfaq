#pragma once
#include <Arduino.h>
#include <WiFiUdp.h>

#define WIFIudp_localport 8888  // порт для служебного UDP

extern WiFiUDP WIFIudp;          // основной UDP для системных задач (NTP, пинг и т.д.)
extern int8_t WIFIudp_flag;      // <0 — не инициализирован, 0 — свободен, 1/123/124 — занят операцией
extern uint32_t WIFIudp_timestamp; // момент последнего обмена

// extern WiFiUDP MERAudp;          // отдельный UDP для “одноразовых” отправок (без приёма)
// extern byte MERA_status;         // служебный статус
extern WiFiUDP MEudp;
extern byte ME_status;

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

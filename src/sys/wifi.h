#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include "config.h"
#include "ARG.h"

#ifdef ESP32
  #include <WiFi.h>
  #include <WebServer.h>
  #include <esp_wifi.h>
  #include <esp_netif.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
#endif

#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Ticker.h>
#include <SPIFFS.h>
#include <MD5Builder.h>

// --- Внешние зависимости (другие модули проекта) ---
#include "LOG.h"
#include "file.h"


// #include "MOTO.h"
// #include "play.h"
// #include "udp.h"

#include <WiFiClient.h>
// если уже есть интерфейс (но может быть поднята своя сеть и нет выхода никуда)
#define ISWIFI (WifiMode>0 && WiFi.status()==WL_CONNECTED)
// если подключились к местной сети
#define ISNET (WifiMode&1 && WiFi.status()==WL_CONNECTED)


// === Глобальные переменные ===
extern byte WifiMode;  // 0 - не инициализировано, 1 - STA, 2 - AP, 3 - AP_STA

// === Прототипы функций ===

// подключение WiFi с ожиданием
bool WIFItryConnect(String ssid, String pass);

#ifdef ESP32
// обработка событий WiFi
void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);

// утилиты для форматирования MAC/IP
String getBytesIP(esp_ip4_addr_t i);
String getBytesMAC(uint8_t* m);
#endif

#endif // WIFI_H

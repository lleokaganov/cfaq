#ifndef DOMOTO_H
#define DOMOTO_H

#include <Arduino.h>


#include "module/LED_WS/h.h"
#include "module/TFT_SPI/h.h"


































#include "LOG.h"
// #include "file.h"
#include "MOTO.h"
// #include "REPER.h"
#include "shedule.h"
#include "main.h"
#include "upgrade.h"
#include "FLT.h"
#include "timer.h"
#include "udp.h"


// Поддержка web
#ifdef ESP32
  #include <WiFi.h>
  #include <WebServer.h>
  extern WebServer WEB;
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  extern ESP8266WebServer WEB;
#endif

#include <WiFiUdp.h>
#include <Ticker.h>
#include <FS.h>
#include <SPIFFS.h>

// --- Глобальные внешние переменные из main.cpp ---
extern byte WifiMode;
extern byte webstart;
extern WiFiUDP WIFIudp;
extern WiFiUDP MEudp;
extern uint32_t WIFIudp_timestamp;
extern int8_t WIFIudp_flag;
extern unsigned long UnixTime;


// extern String EchoMOTO;
// extern String SheduleS;

// === Прототипы функций ===

// Главная команда-интерпретатор
String DOMOTO(String s);
String DOMOTO(String s, byte yild);

// Утилитарные помощники
String OKER(bool x);
String IOKER(bool x);

#endif

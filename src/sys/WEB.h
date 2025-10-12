#ifndef WEB_H
#define WEB_H

#include <Arduino.h>

#include "LOG.h"
#include "file.h"
#include "templates.h"

#include "esp_netif.h"

// Внешние переменные, если есть (например, WEB — глобальный сервер)


#ifdef ESP32
    #include <WiFi.h>
    #include <WebServer.h>
    extern WebServer WEB;
//     #include "SPIFFS.h"
//     #include <HTTPClient.h>
//     #include <WiFiClientSecure.h>
#else
    // WiFiClient wifiClient;
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
    extern ESP8266WebServer WEB( DEFAULTPORT );
//     #include <ESP8266HTTPClient.h>
#endif


// Прототипы всех функций из WEB.cpp
String web_find(String &s, String from, String to);

void WEB_init();

void WEB_MOTO_POST();
void WEB_MOTO();
void web_moto(String s);
void WEB_DIR();
void WEB_INFO();
void WEB_FM();
void WEB_NOTFOUND();
String RE(String n);
int RE0(String n);
void WEBNAH();
String getContentType(String filename);
void otprav(String s);
bool filewebsend(String f);
bool AD();
bool AD(bool i);
void handleFileUpload();

#endif

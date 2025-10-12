#ifndef FILEGET_H
#define FILEGET_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include "LOG.h"
#include "wifi.h"

#ifdef ESP32
    // Выбираем HTTP или HTTPS
    #include <WiFiClientSecure.h>
#endif

// Основные HTTP-запросы
String file_get_contents(String url);
String file_get_contents(String url, uint16_t timeout);

String file_post_contents(String url, String data);
String file_post_contents(String url, String data, uint16_t timeout);
String file_post_contents(String url, String data, uint16_t timeout, String mode);

String file_fetch_contents(String method, String url, String data, uint16_t timeout, String mode);

// Вспомогательные функции
String CHURL(String url);

#endif  // FILEGET_H

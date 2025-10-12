#ifndef UPGRADE_H
#define UPGRADE_H

#include <Arduino.h>
#include "ARG.h"
#include "LOG.h"
#include "udp.h"
#include "file.h"


// #include "utils.h" // Замените на ваш файл с функциями: getfile, ARG, Slash и прочие.


#ifdef ESP32
    // #include <ESP32httpUpdate.h>
    #include <HTTPUpdate.h>
    #include <MD5Builder.h>
#else
    #include <ESP8266httpUpdate.h>
#endif


/*
#include <WiFiClient.h>
#ifdef ESP32
  #include <HTTPUpdate.h>
  using namespace esp32httpupdate;
#else
  #include <ESP8266HTTPUpdate.h>
#endif
*/

// Прототипы функций
// bool inList(String file, String list);
bool upgrade_url(String file);
bool UpgradeALL(bool tcpudp);
bool UpgradeALL_res(String otv);

#ifdef ESP32
void HTTPupdate_started();
void HTTPupdate_finished();
void HTTPupdate_progress(int cur, int total);
void HTTPupdate_error(int err);
#endif

#endif // UPGRADE_H

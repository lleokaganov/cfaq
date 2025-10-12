#ifndef FILE_H
#define FILE_H

#include <Arduino.h>

#include <SPIFFS.h>
#include <HTTPClient.h>
#include <MD5Builder.h>

#include "LOG.h"
#include "fileget.h"
#include "wifi.h"

/*
#include <FS.h>
#ifdef ESP32
#include <SPIFFS.h>
#else
#include <LittleFS.h>
#define SPIFFS LittleFS
#endif
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <MD5Builder.h>
*/

bool inList(String file, String list);

// Функции работы с файлами
unsigned long filesize(String file);
String noSlash(String s);
String Slash(String s);
bool is_file(String f);
bool file_rename(String f, String t);
bool file_del(String file);
bool file_save(String file, String s);
bool file_save(String file, String s, char* mod);
String getfile(String file);
bool file_copy(String from, String to);

// Функции для работы с бинарными файлами
int8_t file_upload_binary(String url, String filename);
int8_t file_upload_binary(String url, String filename, String md5);
String MD5file(String file);

#endif // FILE_H

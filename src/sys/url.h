#pragma once
#include <Arduino.h>

struct url_struct {
  String protocol;
  String host;
  uint16_t port;
  String path;
};

url_struct url_parse(String s);

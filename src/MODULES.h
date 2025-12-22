#pragma once
#include <Arduino.h>

#define CMD_EQ(x) (!strcmp_P(cmd_c, PSTR(x)))
static const String EMPTY_STRING;

#ifdef USE_UDP
    #include "module/udp/h.hpp"
#endif

#ifdef USE_UPGRADE_OTA
    #include "module/upgrade_ota/h.hpp"
#endif

#ifdef USE_QR_CODE
    #include "module/qr-code/h.hpp"
#endif

#ifdef USE_WS_CLIENT
  #include "module/ws-client/h.hpp"
#endif

#ifdef USE_LED_WS
  #include "module/LED_WS/h.hpp"
#endif

#ifdef USE_TFT_SPI
  #include "module/TFT_SPI/h.hpp"
#endif

#ifdef USE_LCD_I2C
  #include "module/LCD_I2C/h.hpp"
#endif

#ifdef USE_DS18B20
  #include "module/DS18B20/h.hpp"
#endif

#ifdef USE_ONEWIRE
  #include "module/ONEWIRE/h.hpp"
#endif

#ifdef USE_I2C
  #include "module/I2C/h.hpp"
#endif

#ifdef USE_TFT_ST7796
  #include "module/TFT_ST7796/h.hpp"
#endif

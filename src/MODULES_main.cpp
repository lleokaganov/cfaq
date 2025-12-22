#include "MODULES.h"


#ifdef USE_UDP
  #include "module/udp/MAIN.inc"
#endif

#ifdef USE_UPGRADE_OTA
    #include "module/upgrade_ota/MAIN.inc"
#endif

#ifdef USE_QR_CODE
    #include "module/qr-code/MAIN.inc"
#endif

#ifdef USE_WS_CLIENT
    #include "module/ws-client/MAIN.inc"
#endif

#ifdef USE_TFT_ST7796
    #include "module/TFT_ST7796/MAIN.inc"
#endif

#ifdef USE_ENCODER
#include "module/ENCODER/MAIN.cpp"
#endif

#ifdef USE_WIEGAND
#include "module/wiegand/MAIN.cpp"
#endif

#ifdef USE_RC522
#include "module/RC522/MAIN.cpp"
#endif

#ifdef USE_MQTT
#include "module/MQTT/MAIN.cpp"
#endif

#ifdef USE_DS18B20
    #include "module/DS18B20/MAIN.inc"
#endif

#ifdef USE_ONEWIRE
    #include "module/ONEWIRE/MAIN.inc"
#endif

#ifdef USE_SPI
#include "module/SPI/MAIN.inc"
#endif

#ifdef USE_TFT_SPI
    #include "module/TFT_SPI/MAIN.inc"
#endif

#ifdef USE_I2C
    #include "module/I2C/MAIN.inc"
#endif

#ifdef USE_LCD_I2C
    #include "module/LCD_I2C/MAIN.inc"
#endif

#ifdef USE_UART
#include "module/UART/MAIN.cpp"
#endif

// #ifdef USE_TFT_ST7796
//     #include "module/TFT_ST7796/MAIN.inc"
// #endif

#ifdef USE_NFC_RF430
#include "module/NFC_RF430/RF430CL.h"
#include "module/NFC_RF430/RF430CL.cpp"
#include "module/NFC_RF430/NDEF.h"
#include "module/NFC_RF430/NDEF.cpp"
#include "module/NFC_RF430/NDEF_TXT.h"
#include "module/NFC_RF430/NDEF_TXT.cpp"
#include "module/NFC_RF430/NDEF_URI.h"
#include "module/NFC_RF430/NDEF_URI.cpp"
#include "module/NFC_RF430/MAIN.cpp"
#endif

#ifdef USE_NFC_PN532
    #include "module/NFC_PN532/MAIN.cpp"
#endif

#ifdef USE_LED_WS
    #include "module/LED_WS/MAIN.inc"
#endif

#ifdef USE_CAMERA
#include "module/CAMERA/MAIN.cpp"
#endif

#ifdef USE_TFT_LGFX
#include "module/TFT_LGFX/MAIN.cpp"
#endif

// Projects

#ifdef USE_IBAN
    #include "projects/IBAN/MAIN.cpp"
#endif


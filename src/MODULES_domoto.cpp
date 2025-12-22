#ifdef USE_UDP
    #include "module/udp/DOMOTO.inc"
#endif

#ifdef USE_UPGRADE_OTA
    #include "module/upgrade_ota/DOMOTO.inc"
#endif

#ifdef USE_QR_CODE
    #include "module/qr-code/DOMOTO.inc"
#endif
  
  #ifdef USE_WS_CLIENT
      #include "module/ws-client/DOMOTO.inc"
  #endif

  #ifdef USE_TFT_SPI
      #include "module/TFT_SPI/DOMOTO.inc"
  #endif

  #ifdef USE_TFT_ST7796
      #include "module/TFT_ST7796/DOMOTO.inc"
  #endif

  #ifdef USE_I2C
      #include "module/I2C/DOMOTO.inc"
  #endif

  #ifdef USE_LCD_I2C
        #include "module/LCD_I2C/DOMOTO.inc"
  #endif

  #ifdef USE_LED_WS
    #include "module/LED_WS/DOMOTO.inc"
  #endif

  #ifdef USE_DS18B20
      #include "module/DS18B20/DOMOTO.inc"
  #endif

  #ifdef USE_ONEWIRE
      #include "module/ONEWIRE/DOMOTO.inc"
  #endif

  // ===================================



  #ifdef USE_IBAN
      #include "projects/IBAN/DOMOTO.cpp"
  #endif

  #ifdef USE_UART
      #include "module/UART/DOMOTO.cpp"
  #endif

  #ifdef USE_WIEGAND
      #include "module/wiegand/DOMOTO.cpp"
  #endif

  #ifdef USE_RC522
      #include "module/RC522/DOMOTO.cpp"
  #endif

  #ifdef USE_MQTT
      #include "module/MQTT/DOMOTO.cpp"
  #endif


  #ifdef USE_SPI
      #include "module/SPI/DOMOTO.cpp"
  #endif


  #ifdef USE_ENCODER
      #include "module/ENCODER/DOMOTO.cpp"
  #endif


  #ifdef USE_NFC_RF430
      #include "module/NFC_RF430/DOMOTO.cpp"
  #endif

  #ifdef USE_NFC_PN532
      #include "module/NFC_PN532/DOMOTO.cpp"
  #endif


  #ifdef USE_CAMERA
        #include "module/CAMERA/DOMOTO.cpp"
  #endif

  #ifdef USE_TFT_LGFX
        #include "module/TFT_LGFX/DOMOTO.cpp"
  #endif

// ============ end of file ==================
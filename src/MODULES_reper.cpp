#ifdef USE_WS_CLIENT
    #include "module/ws-client/REPER.inc"
#endif

#ifdef USE_TFT_ST7796
    #include "module/TFT_ST7796/REPER.inc"
#endif

    #ifdef USE_UART
      #include "module/UART/REPER.cpp"
    #endif

    #ifdef USE_WIEGAND
      #include "module/wiegand/REPER.cpp"
    #endif

    #ifdef USE_RC522
      #include "module/RC522/REPER.cpp"
    #endif

//    #ifdef USE_SPI
//      #include "module/wiegand/SPI.cpp" нет такого
//    #endif

    #ifdef USE_MQTT
      #include "module/MQTT/REPER.cpp"
    #endif

    #ifdef USE_ONEWIRE
      #include "module/ONEWIRE/REPER.inc"
    #endif

    #ifdef USE_I2C
      #include "module/I2C/REPER.inc"
    #endif

    #ifdef USE_DS18B20
      #include "module/DS18B20/REPER.inc"
    #endif

    #ifdef USE_KY_IIC_3V3
      #include "module/KY-IIC-3V3/REPER.inc"
    #endif












    #ifdef USE_ENCODER
      #include "module/ENCODER/REPER.cpp"
    #endif

    #ifdef USE_NFC_RF430
      #include "module/NFC_RF430/REPER.cpp"
    #endif

    #ifdef USE_NFC_PN532
      #include "module/NFC_PN532/REPER.cpp"
    #endif

    #ifdef USE_RFID2
      #include "module/RFID2/REPER.cpp"
    #endif


    #ifdef USE_IBAN
      #include "projects/IBAN/REPER.cpp"
    #endif


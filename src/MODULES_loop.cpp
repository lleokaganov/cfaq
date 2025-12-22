#ifdef USE_WS_CLIENT
      #include "module/ws-client/LOOP.inc"
#endif

#ifdef USE_ENCODER
  #include "projects/ENCODER/LOOP.cpp"
#endif

#ifdef USE_IBAN
  #include "projects/IBAN/LOOP.cpp"
#endif

#ifdef USE_MQTT
  #include "module/MQTT/LOOP.cpp"
#endif

#ifdef USE_NFC_RF430
  #include "module/NFC_RF430/LOOP.cpp"
#endif

#ifdef USE_CAMERA
#ifdef USE_TFT_LGFX
  #include "module/CAMERA/LOOP.cpp"
#endif
#endif

#ifdef USE_WEB
  if(webstart) WEB.handleClient();
#endif

#ifdef USE_TFT_ST7796
  #include "module/TFT_ST7796/LOOP.inc"
#endif

  /// это говно только под ESP12
  /// SOFT.serial RX TX BAUD

  if(cmd=="UART.init") { // UART.init 16 17 115200
    byte RX = PARG0(s,1);
    byte TX = PARG0(s,2);
    uint32_t BAUD = PARG0(s,3);
    LOGI(LOG_MAIN, cmd+":"+String(RX)+" TX:"+String(TX)+" BAUD:"+String(BAUD));
    #ifdef ESP32
      SerialX.begin(BAUD, SERIAL_8N1, RX, TX);
    #else
      SerialX.begin(BAUD, SWSERIAL_8N1, RX, TX, false, 128);
    #endif
    return "";
  }

  if(cmd=="UART.write") {
    String o=REPER(ARG_OTHER(s,0));
    // LOGI(LOG_MAIN, "SOFT.write ["+o+"]");
    o.replace("\\n","\n");
    SerialX.write( o.c_str() );
    return "";
  }

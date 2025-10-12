
 if(l == F("UART")) {
    String S="";
    while(SerialX.available()) {
       char c = SerialX.read();
       S += c;
       if(c == '\n') break;
    }
    return S;
  }

 if(l == F("UARTS")) {
    if(SerialX.available()) return SerialX.readString();
    return "";
  }

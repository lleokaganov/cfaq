// === NTP & UDP ===

if(CMD_EQ("NTP.update")) { // установить часы
  if(!ISNET) return EMPTY_STRING;
  if(WIFIudp_flag < 0) { WIFIudp.begin(WIFIudp_localport); WIFIudp_flag=0; } // запустить сервак
  else if(WIFIudp_flag != 0) return EMPTY_STRING; // занято пока
  WIFIudp_flag = 123;
  String url=PARG(s,1); if(url=="") url="time.nist.gov";
  // IPAddress timeServerIP; WiFi.hostByName(url.c_str(), timeServerIP);
  #define NTP_PACKET_SIZE 48 // NTP time stamp is in the first 48 bytes of the message
  byte Bu[ NTP_PACKET_SIZE ]; memset(Bu, 0, NTP_PACKET_SIZE);
  Bu[0] = 0b11100011;   // LI, Version, Mode
  Bu[1] = 0;     // Stratum, or type of clock
  Bu[2] = 6;     // Polling Interval
  Bu[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  Bu[12]  = 49;
  Bu[13]  = 0x4E;
  Bu[14]  = 49;
  Bu[15]  = 52;
  WIFIudp.beginPacket(url.c_str(), 123);
  WIFIudp.write(Bu, NTP_PACKET_SIZE);
  WIFIudp.endPacket();
  WIFIudp_timestamp=ESP.getCycleCount(); // засечем время
  return EMPTY_STRING;
}

if(CMD_EQ("UDP.send")) { // отправить пакет UDP без ответа UDP.send 10.8.0.72:8313 The Text Of Packet for Sending
   if(!ISNET) return EMPTY_STRING; // занято пока
   String UdpAddr=PARG(s,1); String UdpPort=PARG(UdpAddr,1,":"); UdpAddr=PARG(UdpAddr,0,":"); String UdpPack=PARG_OTHER(s,1);
    LOG(cmd+" "+UdpAddr+":"+UdpPort+" SEND:["+UdpPack+"]");
   MEudp.beginPacket( UdpAddr.c_str(), UdpPort.toInt() );
   MEudp.printf(UdpPack.c_str()); // MEudp.write( UdpPack.c_str(), UdpPack.length() );
   MEudp.endPacket();
   return EMPTY_STRING;
}

if(CMD_EQ("UDP.ping")) { // отправить пакет и выполнить ответ UDP.ping 10.8.0.72:8313 The Text Of Packet for Sending
  if(!ISNET) return EMPTY_STRING;
  if(WIFIudp_flag < 0) { WIFIudp.begin(WIFIudp_localport); WIFIudp_flag=0; } // запустить сервак
  else if(WIFIudp_flag != 0) return EMPTY_STRING; // занято пока
  WIFIudp_flag = 124;
  String UdpAddr=PARG(s,1); String UdpPort=PARG(UdpAddr,1,":"); UdpAddr=PARG(UdpAddr,0,":"); String UdpPack=PARG_OTHER(s,1);
    LOG(cmd+" "+UdpAddr+":"+UdpPort+" SEND:["+UdpPack+"]");
  WIFIudp.beginPacket(UdpAddr.c_str(), UdpPort.toInt());
  WIFIudp.printf(UdpPack.c_str()); // WIFIudp.write(Bu, NTP_PACKET_SIZE);
  WIFIudp.endPacket();
  WIFIudp_timestamp=ESP.getCycleCount(); // засечем время
  return EMPTY_STRING;
}

if(CMD_EQ("UDP.get")) { // отправить пакет и получить ответ в UDP_result, выполнить процедуру UDP_func
  if(!ISNET) return EMPTY_STRING;
  if(WIFIudp_flag < 0) { WIFIudp.begin(WIFIudp_localport); WIFIudp_flag=0; } // запустить сервак
  else if(WIFIudp_flag != 0) return EMPTY_STRING; // занято пока
  WIFIudp_flag = 1;
  String UdpAddr=PARG(s,1); String UdpPort=PARG(UdpAddr,1,":"); UdpAddr=PARG(UdpAddr,0,":"); String UdpPack=PARG_OTHER(s,1);
    LOG(cmd+" "+UdpAddr+":"+UdpPort+" SEND:["+UdpPack+"]");
  WIFIudp.beginPacket(UdpAddr.c_str(), UdpPort.toInt());
  WIFIudp.printf(UdpPack.c_str()); // WIFIudp.write(Bu, NTP_PACKET_SIZE);
  WIFIudp.endPacket();
  WIFIudp_timestamp=ESP.getCycleCount(); // засечем время
  return EMPTY_STRING;
}


#include <Arduino.h>
#include "upgrade.h"


#ifdef ESP32
  void HTTPupdate_started() { LOGI(LOG_UPGRADE,"CALLBACK: HTTP update process started"); }
  void HTTPupdate_finished() { LOGI(LOG_UPGRADE,"CALLBACK:  HTTP update process finished"); }
  void HTTPupdate_progress(int cur, int total) { LOGI(LOG_UPGRADE,"CALLBACK:  HTTP update process at "+String(cur)+" of "+String(total)+" bytes..."); }
  void HTTPupdate_error(int err) { LOGI(LOG_UPGRADE,"CALLBACK:  HTTP update fatal error code "+String(err)); }
#endif

bool upgrade_url(String file) {
  if(!ISNET) return false; // если нет подключения к сети
  if(inList(file, "stoplist.txt")) return 0;
  t_httpUpdate_return ret;
  LOGI(LOG_UPGRADE, "Update new sketch " + file);
    // ret = ESPhttpUpdate.update(file);
    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    WiFiClient wificlient;

  #ifdef ESP32
    httpUpdate.onStart(HTTPupdate_started);
    httpUpdate.onEnd(HTTPupdate_finished);
    httpUpdate.onProgress(HTTPupdate_progress);
    httpUpdate.onError(HTTPupdate_error);

    httpUpdate.rebootOnUpdate(true);
    #ifdef LED_BUILTIN
       httpUpdate.setLedPin(LED_BUILTIN, LOW);
    #endif
    ret = httpUpdate.update(wificlient,file); // ret = ESPhttpUpdate.update(wificlient,file);  //ret = httpUpdate.update(client, "server", 80, "/file.bin");
  #else
    ESPhttpUpdate.rebootOnUpdate(true);
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
    ret = ESPhttpUpdate.update(wificlient,file);
  #endif

  switch(ret) {
      case HTTP_UPDATE_FAILED:
{
#ifdef ESP32
    LOGI(LOG_UPGRADE,"HTTP_UPDATE_FAILED Error ("+String(httpUpdate.getLastError())+"): "+httpUpdate.getLastErrorString());
#else
    String err = ESPhttpUpdate.getLastErrorString();
    if (err.indexOf("reset") >= 0) {
      LOGI(LOG_UPGRADE, "Press RESET and repeat!");
      ESP.restart();
    } else {
      LOGI(LOG_UPGRADE, "UPDATE ERROR [" + file + "]: " + err);
    }
#endif
}
    break;
    case HTTP_UPDATE_NO_UPDATES: LOGI(LOG_UPGRADE, "HTTP_UPDATE_NO_UPDATES"); break;
    case HTTP_UPDATE_OK: LOGI(LOG_UPGRADE, "HTTP_UPDATE_OK"); break;
    default: LOGI(LOG_UPGRADE, "OTHER"); break;
  }

  return 1;
}

bool UpgradeALL(bool tcpudp) {
  if(!ISNET) return false; // если нет подключения к сети

  if(!tcpudp) {
    String upd = CF("server_update"); if(upd == "") return false;
    return UpgradeALL_res( file_get_contents(upd, 3000) );
  }

  // по UDP

  if(WIFIudp_flag < 0) { WIFIudp.begin(WIFIudp_localport); WIFIudp_flag=0; } // запустить сервак
  else if(WIFIudp_flag != 0) return false; // занято пока
  WIFIudp_flag = 125;
  String UdpAddr=CF("server_udp_update"); if(UdpAddr=="") return false;
  String UdpPort=PARG(UdpAddr,1,":"); UdpAddr=PARG(UdpAddr,0,":"); String UdpPack=String(WiFi.macAddress())+" UPGRADE "+CF("soft");
  WIFIudp.beginPacket(UdpAddr.c_str(),UdpPort.toInt());
  WIFIudp.printf(UdpPack.c_str()); // WIFIudp.write(Bu, NTP_PACKET_SIZE);
  WIFIudp.endPacket();
  WIFIudp_timestamp=ESP.getCycleCount(); // засечем время
  return true;
}

bool UpgradeALL_res(String otv) {
  if(ARG(otv, 0, "\n") != "OK") return false;
  String upd = CF("server_update"); if(upd == "") return false;
  String firmware_md5 = "";
  bool ResetNeed = 0;
  int i = 1; while (1) {
    String s = ARG(otv, i++, "\n"); if (s=="") break;
    String file = ARG(s,0," ");
    if(inList(file, "stoplist.txt")) continue;
    String md5 = ARG(s,1," ");
    if(file=="firmware.bin") { firmware_md5 = md5; continue; }
    if(MD5file(file) == md5) continue;
    LOGI(LOG_UPGRADE, "UPDATE ["+upd+"&file="+file+"]: ");
    int i = file_upload_binary(upd + "&file=" + file, file, md5);
    LOGI(LOG_UPGRADE, " => ");
    if(!i && MD5file(file) == md5) {
      LOGI(LOG_UPGRADE, "OK", "FFMM");
	String upinfo = "set LastUpgradeFile = " + file;
	if(is_file("/upgrade.callback")) upinfo = upinfo + "\n/upgrade.callback";
	MOTO(upinfo);
      if(file == "config.txt") ResetNeed = 1;
    } else {
      LOGI(LOG_UPGRADE, "\nERRORe #" + String(i) + " [" + upd + "&file=" + file + "]", "DRMDRD");
      return false;
    }
  }
  if(firmware_md5 != "" && firmware_md5 != ESP.getSketchMD5() ) {
    LOGI(LOG_UPGRADE, "firmware.bin need to UPDATE", "mmmrdCL.LdCLSLMRM");
    MOTO("set LastUpgradeFile = firmware.bin\n/upgrade.callback");
    upgrade_url(upd + "&file=firmware.bin");
  }
  if(ResetNeed) {
    LOGI(LOG_UPGRADE, "\n\nRESET NEED...\n", "dSdSdCC");
    ESP.restart();
  }
  return true;
}


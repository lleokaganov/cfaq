#include <Arduino.h>
#include "main.h"

byte WifiMode = 0; // состояние Wifi: 0 - не инициализировано, 1 - STA, 2 - AP, 3 - AP_STA

#ifdef ESP32

String getBytesIP(esp_ip4_addr_t i) {
 char o[]="192.168.111.222\0x00";
 sprintf(o,"%d.%d.%d.%d",((byte*)&i)[0],((byte*)&i)[1],((byte*)&i)[2],((byte*)&i)[3]);
 return String(o);
}

String getBytesMAC(uint8_t* m) {
 char o[]="11:22:33:44:55:66\0x00";
 sprintf(o,"%02X:%02X:%02X:%02X:%02X:%02X",m[0],m[1],m[2],m[3],m[4],m[5]);
 return String(o);
}

void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
  String o="";
  String s="";
       if(event == ARDUINO_EVENT_WIFI_SCAN_DONE)                s="WIFI_SCAN_DONE";
  else if(event == ARDUINO_EVENT_WIFI_STA_CONNECTED)            s="WIFI_STA_CONNECTED";
  else if(event == ARDUINO_EVENT_WIFI_STA_GOT_IP)               s="WIFI_STA_GOT_IP";
  else if(event == ARDUINO_EVENT_WIFI_AP_STACONNECTED)          s="WIFI_AP_STACONNECTED";
  else if(event == ARDUINO_EVENT_WIFI_AP_STADISCONNECTED)       s="WIFI_AP_STADISCONNECTED";
  else if(event == ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED)         { s="WIFI_AP_STAIPASSIGNED"; o=getBytesIP(info.wifi_ap_staipassigned.ip); }
  else if(event == ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED)        s="WIFI_AP_PROBEREQRECVED";
  else if(event == ARDUINO_EVENT_WIFI_FTM_REPORT)               s="WIFI_FTM_REPORT";
  else return;
  if(o!="") CFSET("WIFI_EVENT_INFO",o);
  s=CF(s);
  if(s!="") loop_now(s);
}
#endif


bool WIFItryConnect(String ssid, String pass) {
  WiFi.begin(ssid.c_str(), pass.c_str());
  byte w;
  for (uint8_t i = 0; i < 20; i++) {
    LOGI(LOG_MAIN, ".");
    delay(500);
    w = WiFi.status();
    if (w == WL_CONNECTED) {
      LOGI(LOG_MAIN, "Connected \"" + ssid + "\" IP: " + WiFi.localIP().toString() );
      return 1;
    }
    else if (w == WL_IDLE_STATUS) LOGI(LOG_ERROR, F("\n WiFi is in process of changing between statuses"));
    else if (w == WL_NO_SSID_AVAIL) LOGI(LOG_ERROR, F("\n SSID cannot be reached"));
    else if (w == WL_CONNECT_FAILED) LOGI(LOG_ERROR, F("\n password is incorrect"));
    // else if(w == WL_DISCONNECTED) LOGI(LOG_ERROR,F("\n module is not configured in station mode"));
  }

  LOGI(LOG_ERROR, " ERROR connect to [" + ssid + "]");
  return 0;
}

/*
void WiFiEvent(WiFiEvent_t event) {
  String cb = CF("WIFI.callback"); if (cb == "") return;
  CFSET("WIFI.event", String(event)); loop_new(1, 255, 1, 1, cb);
}
*/

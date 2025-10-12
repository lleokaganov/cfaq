#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <Arduino.h>

#ifdef ESP32
const char DefaultConfig[] PROGMEM = R"rawliteral(

echo Version: {VER} {VERNAME}
pinmode 2 OUTPUT
pin 2 0
set AP_password =
WIFI.disconnect YES
WIFI.APdisconnect YES
set soft = default5
set AP_name = ESP_HELP
usleep 200
set server_update = http://lleo.me/ESP8266/index.php?ip=<<ip>>&chip=<<chip>>&soft=<<soft>>
WIFI.autoconnect YES
WIFI.autoreconnect YES
WIFI.persistent FALSE
WIFI.mode STA
WIFI.dns NO

if.empty {REPARE} {
  loop 1 10 upgrade
  set TMP = {FILE:/wifi_last.txt}
  if.!empty {TMP} {
    WIFI {PARSE:TMP|*\\n} {PARSE:TMP|\\n*}
    WIFI.waitconnect
  }
} else echo REPARE MODE

if.!WIFI {
  WIFI D922 77777701
  WIFI.waitconnect
}

if.WIFI {
  echo Connected {ip_ssid} http://{ip}
  playip
} else {
  WIFI.mode AP
  WIFI.hostname {AP_name}
  WIFI.AP {AP_name}
  WIFI.dns
  echo WIFI: {AP_name}
}

)rawliteral";

#else

const char DefaultConfig[] PROGMEM = R"rawliteral(

#endifecho Version: {VER} {VERNAME}
pinmode 2 OUTPUT
pin 2 0
set AP_password =
WIFI.disconnect YES
WIFI.APdisconnect YES
set soft = default5
set AP_name = ESP_HELP
usleep 500
set server_update = http://lleo.me/ESP8266/index.php?ip=<<ip>>&chip=<<chip>>&soft=<<soft>>
WIFI.autoconnect YES
WIFI.autoreconnect YES
WIFI.persistent FALSE
WIFI.mode STA
WIFI.dns NO

if.empty {REPARE} {
  loop 1 10 upgrade
  set TMP = {FILE:/wifi_last.txt}
  if.!empty {TMP} {
    WIFI {PARSE:TMP|*\\n} {PARSE:TMP|\\n*}
    WIFI.waitconnect
  }
} else echo REPARE MODE

if.!WIFI {
  WIFI D922 77777701
  WIFI.waitconnect
}

if.WIFI {
  echo Connected {ip_ssid} http://{ip}
  playip
} else {
  WIFI.mode AP
  WIFI.hostname {AP_name}
  WIFI.AP {AP_name}
  WIFI.dns
  echo WIFI: {AP_name}
}

)rawliteral";

#endif




const char DefaultINDEX[] PROGMEM = R"rawliteral(
<html><body>
<h1>WiFi</h1>
<p><div style='font-size:24px;'>

<form method='GET' action='/FM'>
    <input type='hidden' name='a' value='WIFIconn1'>
    <br>Login: <input type='text' name='net'>
    <br>Password: <input type='text' name='pass'>
<br><input type='submit' value='Go'>
</form>

</div>
</body></html>
)rawliteral";

const char InfoTemplate[] PROGMEM = R"rawliteral(

soft: {soft} chip: {chip}
CpuFreq: {CpuFreq} MHz FreeHeap: {FreeHeap} FlashChipRealSize: {FlashChipRealSize}
 ip_ssid: {ip_ssid}
 ip: {ip}
mdns: http://{mdns}.local
 softAPIP: {softAPIP}
 AP_name: {AP_name}
 AP_password: {AP_password}

ResetReason: {ResetReason}

)rawliteral";

#endif // TEMPLATES_H
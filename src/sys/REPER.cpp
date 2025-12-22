#include <Arduino.h>
#include "main.h"
#include "MODULES.h"

int dBm2percent(int d) { return (d <= -100 ? 0 : (d >= -50 ? 100 : 2*(d+100) )); }

#if defined(CONFIG_IDF_TARGET_ESP32)
  extern "C" uint8_t temprature_sens_read();
#endif

String WIFI_all_txt(int n) {
    String S = "";
    if(n) for(int i=0;i<n;i++) {
    #ifdef ESP32
      S += String( WiFi.encryptionType(i)==WIFI_AUTH_OPEN ?"0":"1");
    #else
      S += String( WiFi.encryptionType(i)==ENC_TYPE_NONE ?"0":"1");
    #endif
      S +=" "+String(WiFi.channel(i))
      +" "
#ifndef ESP32
      +(WiFi.isHidden(i)?"1":"0")
#else
      +"1" // БЛЯДИ РОГАТЫЕ ВЫРЕЗАЛИ ПОДДЕРЖКУ isHidden в ESP32
#endif
      +" "+String(WiFi.RSSI(i))
      +" "+String(dBm2percent(WiFi.RSSI(i)))
      +" "+WiFi.BSSIDstr(i)
      +" "+WiFi.SSID(i)+"\n";
    }
    WiFi.scanDelete();
    return S;
}

String REPERFILE(String url) {
  return REPER(getfile(url));
}

// String REPER(String s) {
//   String l, from, to; int a, a2, b, stp = 500;
//   while ( stp-- ) {
//     b = s.indexOf("}"); if (b < 0) return s; // ищем первую закрывающую
//     a = -1; while (1) {
//       a2 = s.indexOf("{", a + 1);  // ищем ближайшую к ней открывающую
//       if (a2 < 0 || a2 > b) break;
//       a = a2;
//     }
//     if (a < 0) return s;
//     l = s.substring(a + 1, b);
//     s.replace( s.substring(a, b + 1) , ( l.indexOf(":") >= 0 ? REPER_complex(l) : REPER_simplex(l) ) );
//   }
//   return s;
// }

const String SAFE_L = "@\x1C%"; // FS
const String SAFE_R = "%\x1D@"; // GS

String REPER(String s) {
  // Serial.println(" [IN:" + s + "]");
  String l, from, to, fnresult;
  int a, a2, b, stp = 500;
  while ( stp-- ) {
    b = s.indexOf("}"); if (b < 0) break; // return s; // ищем первую закрывающую
    a = -1; while (1) {
      a2 = s.indexOf("{", a + 1);  // ищем ближайшую к ней открывающую
      if (a2 < 0 || a2 > b) break;
      a = a2;
    }
    if (a < 0) break; // return s;
    l = s.substring(a + 1, b);
    fnresult = ( l.indexOf(":") >= 0 ? REPER_complex(l) : REPER_simplex(l) );
    if (fnresult == l) fnresult = SAFE_L + l + SAFE_R;
    s.replace( s.substring(a, b + 1) , fnresult );
  }
  // Убираем защитные символы
  s.replace(SAFE_L, "{"); s.replace(SAFE_R, "}");
  // Serial.println(" [OUT:" + s + "]");
  return s;
}








String REPER_complex(String ll) { // сложная команда
  String l=ll;
  l.replace(SAFE_L, "{"); l.replace(SAFE_R, "}");

  String CMD=ARG(l,0,":"); l=ARG_OTHER(l,0,":");

  if(CMD == "chr") { // 123='{' 125='}'
    uint8_t i = PARG0(l,0);
    char c = (char)i;
    return String(c);
  }

  if(CMD == "GET") { // {GET:500 http://lenta.ru/robots.txt}
    uint16_t timeout = PARG0(l,0); if(timeout) l=PARG_OTHER(l,0); else timeout=5000;
    return file_get_contents(l, timeout);
  }

  if(CMD == "REPLACE") { // from to text
    String text = PARG_OTHER(l,1);
    text.replace( ARG(l,0), ARG(l,1) );
    return text;
  }

  if(CMD == "POST") { // {GET:500 http://lenta.ru/robots.txt}
    uint16_t timeout = PARG0(l,0); if(timeout) l=ARG_OTHER(l,0); else timeout=5000;
    String url = PARG(l,0);
    String body = PARG_OTHER(l,0);
    return file_post_contents(url,body,timeout);
  }

  if(CMD == "POSTJSON") { // {GET:500 http://lenta.ru/robots.txt}
    uint16_t timeout = PARG0(l,0); if(timeout) l=ARG_OTHER(l,0); else timeout=5000;
    String url = PARG(l,0);
    String body = PARG_OTHER(l,0);
    return file_post_contents(url,body,timeout,"JSON");
  }

  if(CMD == "FILE") return getfile(REPER(l));
  if(CMD == "FILEMD5") return MD5file(REPER(l));

  if(CMD == "TAKE") {
    String sep = PARG(l, 2);
    sep.replace("\\n", "\n");
    // Serial.println("=== TAKE sep=[" + sep + "] ===");
    return PARG( CF(ARG(l, 0)) , PARG0(l, 1) , (sep == "" ? " " : sep) );
  }

  // if(CMD == "TRIM") {
  //   String s = l;
  //   while (s.startsWith(" ") || s.startsWith("\n") || s.startsWith("\r")) s = s.substring(1);
  //   while (s.endsWith(" ") || s.endsWith("\n") || s.endsWith("\r")) s = s.substring(0, s.length() - 1);
  //   return s;
  // }

  if(CMD == "PARSE") {
    l.replace("|", " ");
    String VAL = CF(ARG(l, 0)); l = ARG_OTHER(l, 0);
    String part = ARG(l, 0, "*"); part.replace("\\n", "\n"); part.replace("\\r", "\r");
    VAL = VAL.substring( part.length() + VAL.indexOf(part) );
    part = ARG(l, 1, "*");
    if (part != "") {
      part.replace("\\n", "\n"); part.replace("\\r", "\r");
      VAL = VAL.substring( 0, VAL.indexOf(part) );
    }
    return VAL;
  }

  if(CMD == "conf") {
    String key = PARG(l,0); if(key == "") return "";
    String S=getfile("/Settings.txt");
    uint16_t x=findkey(S, key);
    if (x==0xFFFF) return PARG(l,1);
    S=ARG(S.substring(x),0,"\n");
    return ARG_OTHER(S,0);
  }

// set sound.pin = {KEY:Settings.txt sound.pin}
// if.empty {sound.pin} sound.pin = 3

  if(CMD == "LOOP") {
    for (byte i = 0; i < NLOOP; i++) {
      if (Loopes[i].value && (l == Loopes[i].file || l == String(i) ) ) return String(Loopes[i].value - Loopes[i].count);
    }
    return "";
  }

  if(CMD == "at") {
    CMD = "LOOP";
    l = "1 " + l;
  }
  if (CMD == "loop") {
    uint32_t one = PARG0(l, 0);
    uint32_t value = PARG0(l, 1);
    if (!value) return String( loop_new(0, 255, one, 0, PARG_OTHER(l, 0)) );
    return String( loop_new(one, 255, value, 0, PARG_OTHER(l, 1)) ); // {NEWLOOP:1 20 /FILE} // 1 раз
  }

  if(CMD == ""){
    String file = "Settints.txt", key=PARG(l,1); if(key=="") return "error";
    String S=getfile(file); uint16_t x=findkey(S, key); if(x!=0xFFFF) { S=ARG(S.substring(x),0,"\n"); return ARG_OTHER(S,0); }
    return PARG(l,2);
  }

  if(CMD == "KEY"){
    String file = PARG(l, 0), key = PARG(l, 1); if (file == "" || key == "") return "error";
    String S=getfile(file);
    uint16_t x=findkey(S, key);
    if (x==0xFFFF) return "";
    S=ARG(S.substring(x),0,"\n");
    return ARG_OTHER(S,0);
  }


  
  
  // if(CMD=="TEST") { return PARG(l,0); }
  // if(CMD=="LOST") { return PARG(l,0); }
 // DISABLEforMERA  if (CMD == "MODBUS") return String(MODBUS.Hreg(l.toInt()));
 // DISABLEforMERA if (l == F("MQTT")) return String(MQTT && client.connected() ? "1" : "0");



  if (CMD == "LOG") {
    return String(LOGISET & (1 << LOG_NAME_I(PARG(l, 0))) ? 1 : 0 );  // NEW
  }

  if (CMD == "URLENCODE") {
    return urlencode(l);
  }

  if (CMD == "HEX") {
    uint32_t n = (uint32_t)PARG0(l,0);
    Serial.println("HEX of [" + l + "] 1 = "+ String(n) +" / " + PARG0(l,0));
    // uint32_t n1 = strtoul(l.c_str(), nullptr, 10);
    // Serial.println("HEX of [" + l + "] 2 = "+ String(n1) );

    String s;
    char buf[3];
    uint8_t *p = (uint8_t*)&n;
    for (int i = sizeof(n)-1; i >= 0; i--) {
      sprintf(buf, "%02X ", p[i]);
      s += buf;
      if(i) s += ' ';
    }
    return s;
  }

  if(CMD == "hh") { return Time_hh( PARG0(l, 0) ); } // NEW
  if(CMD == "mm") { return Time_mm( PARG0(l, 0) ); } // NEW
  if(CMD == "ss") { return Time_ss( PARG0(l, 0) ); } // NEW
  if(CMD == "dn") { return Time_dn( PARG0(l, 0) ); } // день недели 0-6
  if(CMD == "ddn"){ return Time_ddn( PARG0(l, 0) ); } // дни недели su mo tu we th fr sa
  if(CMD == "hhmmss") { return Time_hhmmss( PARG0(l, 0) ); } // NEW

  #include "MODULES_reperX.cpp"

  return ll;
}

// ==================

String REPER_simplex(String ll) {
  String l = ll;
  l.replace(SAFE_L, "{"); l.replace(SAFE_R, "}");

#ifdef ESP8266
  if (l == F("gpioA0")) return String(analogRead(A0));
#endif

#ifdef ESP32
//   if(l.startsWith(F("gpioRAW"))) return String(analogReadRaw( l.substring(7).toInt() ));
  if(l.startsWith(F("gpioA"))) return String(analogRead( l.substring(5).toInt() ));
// if(l.startsWith(F("gpioMV"))) return String(analogReadMilliVolts( l.substring(6).toInt() ));
#endif

if(l.startsWith(F("gpio"))) return String(digitalRead( l.substring(4).toInt() ));

#if defined(CONFIG_IDF_TARGET_ESP32)
 if (l == F("hall_sensor")) return String(hallRead()); // показания датчика Холла
 if (l == F("temp_sensor")) { return String( (temprature_sens_read()-32) / 1.8 ); } // показания датчика температуры
#endif

  // WiFi
  // Точно есть в 
if (l == F("ip_ssid")) return WiFi.SSID();
if (l == F("ip_bssid")) return WiFi.BSSIDstr();
if (l == F("ip_rssi")) return String(WiFi.RSSI()); // ?????????????????

// Точно нет (в ESP32 или теперь вообще в библиотеках):
#ifndef ESP32
  if (l == F("ip_hostname")) return WiFi.hostname();
#endif


  
  if (l == F("ip")) {
    String c = WiFi.localIP().toString();
    return (c.indexOf(".") > 0 ? c : WiFi.softAPIP().toString() );
  }
  if (l == F("macAddress")) return WiFi.macAddress();

if (l == F("AP_list")) {
#ifdef ESP32
    wifi_sta_list_t wifi_sta_list;
    memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));

    // Получаем список клиентов
    if (esp_wifi_ap_get_sta_list(&wifi_sta_list) == ESP_OK) {
        String o = "";
        for (int i = 0; i < wifi_sta_list.num; i++) {
            String mac = getBytesMAC(wifi_sta_list.sta[i].mac);
            String rssi = String(wifi_sta_list.sta[i].rssi);
            // В ESP32 Core 3.x.x нет API для получения IP по MAC, поэтому IP не выводим
            o += mac + " RSSI:" + rssi + "\n";
        }
        return o;
    } else {
        return "Error getting AP list";
    }
#else
    return "Not supported for ESP8266 yet";
#endif
}


// Добавить:
// WiFi.encryptionType(wifiAccessPoint);     TKIP (WPA) = 2    WEP = 5    CCMP (WPA) = 4    NONE = 7    AUTO = 8
// WiFi.getSocket(); // Возвращает первый доступный сокет
// WiFi.macAddress(mac); mac: массив из 6 byte с MAC адресом Возвращаемые значения: 6 byte массив с MAC адресом вашего шилда

// Functions ESP.xxxxx()
#ifndef CODE_MINIMUM

  if (l == F("ip_psk")) return WiFi.psk();
  if (l == F("softAPIP")) return WiFi.softAPIP().toString();
  if (l == F("softAPmacAddress")) return String(WiFi.softAPmacAddress());
  if (l == F("softAPgetStationNum")) return String(WiFi.softAPgetStationNum());
  if (l == F("ip_mask")) return WiFi.subnetMask().toString();
  if (l == F("ip_gateway")) return WiFi.gatewayIP().toString();
  if (l == F("ip_connected")) return String(ISWIFI ? "1" : "0");
#if defined(ESP32)
  if(l == F("ip_autoconnect")) return String(WiFi.getAutoReconnect() ? "1" : "0");
#elif defined(ESP8266)
  if(l == F("ip_autoconnect")) return String(WiFi.getAutoConnect() ? "1" : "0");
#endif
  if (l == F("ip_status")) {
    int16_t i = WiFi.status();
     if (i == WL_CONNECTED) return F("CONNECTED – подключено");
     if (i == WL_NO_SSID_AVAIL) return F("NO_SSID_AVAIL – сеть SSID вне зоны доступа");
     if (i == WL_CONNECT_FAILED) return F("CONNECT_FAILED – ошибка (неверный пароль?)");
     if (i == WL_IDLE_STATUS) return F("IDLE_STATUS – временно заняты подключением");
     if (i == WL_DISCONNECTED) return F("DISCONNECTED – отключен (не в режиме станции)");
     if (i == WL_NO_SHIELD) return F("NO_SHIELD – нет WiFi устройства");
     if (i == WL_SCAN_COMPLETED) return F("SCAN_COMPLETED – сканирование сетей завершено");
     if (i == WL_CONNECTION_LOST) return F("CONNECTION_LOST – соединение потеряно");
    return String("Unknown code: " + String(i));
  }
  if (l == F("ip_dns")) return WiFi.dnsIP(0).toString() + " " + WiFi.dnsIP(1).toString();

  if (l == F("FreeHeap")) return String(ESP.getFreeHeap(), DEC);
  if (l == F("FreeSketchSpace")) return String(ESP.getFreeSketchSpace(), DEC);
  if (l == F("SketchSize")) return String(ESP.getSketchSize(), DEC);
  if (l == F("FlashChipSize")) return String(ESP.getFlashChipSize(), DEC);
  if (l == F("FlashChipSpeed")) return String(ESP.getFlashChipSpeed(), DEC);
  if (l == F("CpuFreq")) return String(ESP.getCpuFreqMHz(), DEC);
  if (l == F("getSketchMD5")) return ESP.getSketchMD5();
  if (l == F("cycles")) return String(ESP.getCycleCount(), DEC);

  if (l == F("flashmode")) { FlashMode_t m = ESP.getFlashChipMode();
    if(m == FM_QIO) return F("QIO");
    if(m == FM_QOUT) return F("QOUT");
    if(m == FM_DIO) return F("DIO");
    if(m == FM_DOUT) return F("DOUT");
#ifdef ESP32
    if(m == FM_FAST_READ) return F("FAST_READ");
    if(m == FM_SLOW_READ) return F("SLOW_READ");
#endif
    if(m == FM_UNKNOWN) return F("UNKNOWN");
    return F("UNKNOWN");
  }

#ifdef ESP32
if(l==F("chip")||l==F("getEfuseMac")) return String(ESP.getEfuseMac(), HEX); //  идентификатор платы, который MAC uint64_t
  if(l==F("SdkVersion")) return ESP.getSdkVersion(); // версия SDK для платы.
  if(l==F("ChipModel")) return ESP.getChipModel();
  if(l==F("ChipRevision")) return String(ESP.getChipRevision(), DEC);      // номер ревизии чипа
  if(l==F("HeapSize")) return String(ESP.getHeapSize(), DEC);          // Internal RAM: полный размер кучи в памяти, bytes.
  if(l==F("MaxAllocHeap")) return String(ESP.getMaxAllocHeap(), DEC);      // Internal RAM: largest block of heap that can be allocated at once, bytes.
  if(l==F("MinFreeHeap")) return String(ESP.getMinFreeHeap(), DEC);       //  Internal RAM: lowest level of free heap since boot, bytes
  if(l==F("PsramSize")) return String(ESP.getPsramSize(), DEC);         //     //SPI RAM
  if(l==F("FreePsram")) return String(ESP.getFreePsram(), DEC);         //     //SPI RAM
  if(l==F("MinFreePsram")) return String(ESP.getMinFreePsram(), DEC);      //     //SPI RAM
  if(l==F("MaxAllocPsram")) return String(ESP.getMaxAllocPsram(), DEC);     //     //SPI RAM
  if(l==F("ChipCores")) return String(ESP.getChipCores(), DEC);         // Ядра?
  // ??? if(l==F("")) return String(ESP.    uint32_t magicFlashChipSize(uint8_t byte);
  // ??? if(l==F("")) return String(ESP.    uint32_t magicFlashChipSpeed(uint8_t byte);
  // ??? if(l==F("")) return String(ESP.    FlashMode_t magicFlashChipMode(uint8_t byte);
#else
if (l == F("chip")) return String(ESP.getChipId()); // c.toUpperCase();
  if (l == F("FlashChipId")) { String c = String(ESP.getFlashChipId(), HEX); c.toUpperCase(); return c; }
  if (l == F("FlashChipRealSize")) return String(ESP.getFlashChipRealSize(), DEC);
  if (l == F("CoreVersion")) return ESP.getCoreVersion();
  if (l == F("FullVersion")) return ESP.getFullVersion();
  if (l == F("BootMode")) return String(ESP.getBootMode(), DEC);
  if (l == F("BootVersion")) return String(ESP.getBootVersion(), DEC);
  if (l == F("FlashChipSizeByChipId")) return String(ESP.getFlashChipSizeByChipId(), DEC);
  if (l == F("ResetReason")) return ESP.getResetReason();
  if (l == F("ResetInfo")) return ESP.getResetInfo();
  if (l == F("vcc")) return String(ESP.getVcc(), DEC);
#endif
  // if (l == F("Files")) {
  //   String o = "";
  //   #ifdef ESP32
  //     File dir = SPIFFS.open("/"); if(!dir || !dir.isDirectory()) return "";
  //     File file;  while( (file = dir.openNextFile()) ){ o += " " + String(file.name()); }
  //   #else
  //     Dir dir = SPIFFS.openDir(""); while(dir.next()) o+=" "+String(dir.fileName());
  //   #endif
  //   o.trim();
  //   return o;
  // }
  // if (l == F("Files_count")) {
  //   int k = 0;
  //   #ifdef ESP32
  //     File dir = SPIFFS.open("/"); if(!dir || !dir.isDirectory()) return "0";
  //     File file; while( (file = dir.openNextFile()) ) k++;
  //   #else
  //     Dir dir=SPIFFS.openDir(""); while(dir.next()) k++;
  //   #endif
  //   return String(k);
  // }

#ifndef ESP32
  if(l==F("SPIFFStotal")) { FSInfo fi; SPIFFS.info(fi); return String(fi.totalBytes); } // size_t-число общего количества Байт размеченных под файловую систему
  if(l==F("SPIFFSused")) { FSInfo fi; SPIFFS.info(fi); return String(fi.usedBytes); } // Этот метод возвращает size_t-число используемых Байт в файловой системе.
  if(l==F("SPIFFSfree")) { FSInfo fi; SPIFFS.info(fi); return String(fi.totalBytes-fi.usedBytes); }
#else
  if(l==F("SPIFFStotal")) return String(SPIFFS.totalBytes());
  if(l==F("SPIFFSused")) return String(SPIFFS.usedBytes());
  if(l==F("SPIFFSfree")) return String(SPIFFS.totalBytes()-SPIFFS.usedBytes());
#endif

  if (l == F("LOOP")) {
    String o = "";
    for (byte i = 0; i < NLOOP; i++) {
      if (Loopes[i].value)
        o += "Loop #" + String(i) + " " + String(Loopes[i].count) + " of " + String(Loopes[i].value) + " [" + Loopes[i].file + "]" + String(Loopes[i].one ? " ONE" : "") + "\n";
    }
    return o;
  }

  if (l == F("SET")) {
    String o = "";
    for (byte i = 0; i < NSET; i++) {
      if (Settes[i].n != "") o += Settes[i].n + " = [" + Settes[i].v + "]\n";
    }
    return o;
  }

#endif




  if (l == F("VER")) return String(VERID);
  if (l == F("VERNAME")) return String(VERNAME);

  if (l == F("ESP_TYPE")) {
   #ifdef ESP32
    #if CONFIG_IDF_TARGET_ESP32S2
      return "ESP_32S2";
    #elif CONFIG_IDF_TARGET_ESP32S3
      return "ESP_32S3";
    #elif CONFIG_IDF_TARGET_ESP32C3
      return "ESP_32C3";
    #else
      return "ESP_32";
    #endif
   #else
      return "ESP_12";
   #endif
  }

#ifdef USE_FLT
  if (l == F("MaxA0")) return String(MaxA0);
  if (l == F("MinA0")) return String(MinA0);
  if (l == F("FltA0")) return String(FltA0);
#endif
// DISABLEforMERA   if (l == F("NSERV")) return String(NSERV);
  if (l == F("NLOOP")) return String(NLOOP);
  if (l == F("NSET")) return String(NSET);
  if (l == F("UnixTime")) return String(UnixTime);
  if (l == F("NBUF")) return String(NBUF);

  if (l == F("hh")) return Time_hh(UnixTime); // часы
  if (l == F("mm")) return Time_mm(UnixTime); // минуты
  if (l == F("ss")) return Time_ss(UnixTime); // секунды
  if (l == F("dn")) return Time_dn(UnixTime); // день недели 0-6
  if (l == F("ddn")) return Time_ddn(UnixTime); // день недели su mo tu we th fr sa
  if (l == F("Time") || l == F("hhmmss")) return Time_hhmmss(UnixTime); // NEW
  if (l == F("millis")) return String(millis()); // NEW

  if (l == F("EchoMOTO")) return EchoMOTO; // NEW
// DISABLEforMERA   if (l == F("mdns")) return String(mdns); // NEW

  if(l == F("LOG")) {
    return String(LOGISET, BIN);
  }

  if(l == F("SERIALSWAP")) {
    return String(SerialSwap);
  }

  if(l == F("SERIAL")) {
    String S="";
    while(Serial.available()) {
       char c = Serial.read();
       S += c;
       if(c == '\n') break;
    }
    return S;
  }

  if (l == F("INFO")) return REPER(InfoTemplate);


  if (l == F("dir")) { // список файлов
    String S = "";

    #ifdef ESP32
        File dir = SPIFFS.open("/");
        if(!dir || !dir.isDirectory()) return "";
        File file; while( (file = dir.openNextFile()) ) {
          if(file.isDirectory()) S += "0 " + String(file.path()) + "/\n";
          else S += String(file.size(), DEC) + " " + String(file.name()) + "\n";
        }
    #else
        Dir dir = SPIFFS.openDir("");
        while (dir.next()) {
          File f = dir.openFile("r");
          S += String(f.size(), DEC) + " " + String(dir.fileName()) + "\n";
        }
    #endif

    return S;
  }

  if(l == F("WiFi.scanComplete")) { int n = WiFi.scanComplete(); return (n < 0 ? F("wait") : WIFI_all_txt(n) ); } // список сетей завершен?
  if(l == F("WIFI.scan")) { return WIFI_all_txt( WiFi.scanNetworks(0,1) );  } // список сетей

  #include "MODULES_reper.cpp"

  String mark = "\x1C!@#!\x1D";
  String l2=CF(l,mark);
  if (l2 != mark) return l2;
  return ll;
}

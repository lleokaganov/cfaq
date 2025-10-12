#include <Arduino.h>
#include "file.h"


bool inList(String file, String list) {
  String s = getfile(list), l;
  while(s.indexOf("\n\n")>=0) s.replace("\n\n","\n");
  //      s=CF(list,"FaLsE"); if(s=="") return 0;
  //if(s=="FaLsE") { s=getfile(list); CFSET(list,s); }
  file = Slash(file);
  int i = 0; while (1) {
    l = ARG(s, i++, "\n"); if (l == "") return 0;
    l=Slash(l);
    if(file == l) {
      // if(Slash(file)!=Slash(list)) LOGI(LOG_UPGRADE, "Deprecated " + file + " in [" + list + "] start [" + l + "]");
      return 1;
    }
  } return 0;
}

unsigned long filesize(String file) {
  file = Slash(file);
  if (!SPIFFS.exists(file)) return 0;
  File f = SPIFFS.open(file, "r"); if (!f) return 0;
  return (unsigned long)f.size();
}

// ====================================================================================
// ====================================================================================
// ====================================================================================
// FILE SPIFFS
String noSlash(String s) {  return ( s.startsWith("/") ? s.substring(1) : s ); }
String Slash(String s) {  return ( s.startsWith("/") ? s : "/" + s); }
bool is_file(String f) {  return SPIFFS.exists(Slash(f)); }
bool file_rename(String f, String t) {  t = Slash(t);  if(is_file(t)) SPIFFS.remove(t);  return SPIFFS.rename(Slash(f), t); }
bool file_del(String file) {  return SPIFFS.remove(Slash(file)); }
bool file_save(String file, String s) { return file_save(file, s, (char*)"w"); }
bool file_save(String file, String s, char* mod) { // “r”, “w”, “a”, “r+”, “w+”, “a+”.
  if(Slash(file)=="/") return 0;
  File f = SPIFFS.open(Slash(file), mod); if(!f) return 0; f.print(s); f.close(); return 1;
}
String getfile(String file) {
  File f = SPIFFS.open(Slash(file), "r");
  if (!f) return "";
  String s = f.readString();
  f.close();
  return s;
}

bool file_copy(String from, String to) { to=Slash(to);
  String tmp=to+".tmp"; if(is_file(tmp)) file_del(tmp);
  #define lenbuf 1024
  uint8_t* duff = (byte*) malloc(lenbuf);
  int i;
  File fp=SPIFFS.open(tmp,"w"); if(!fp) { LOGI(LOG_ERROR, F("Error: file copy")); return 0; }
  File fp_from=SPIFFS.open(from,"r"); if(!fp_from) { LOGI(LOG_ERROR, F("Error: file not found")); return 0; }

  while(fp_from.available() > 0) {
      i = fp_from.readBytes( (char*)duff ,lenbuf);
      if(i<=0 || i>lenbuf) break;
      fp.write(duff,i);
  }

  free(duff);
  fp_from.close();
  fp.close();
  if(is_file(to)) SPIFFS.remove(to);

  return file_rename(tmp,to);
}

int8_t file_upload_binary(String url, String filename) { return file_upload_binary(url, filename, ""); }
int8_t file_upload_binary(String url, String filename, String md5) {
  if(!ISNET) return -99; // если нет подключения к местной сети
  url=CHURL(url);
  filename = Slash(filename); if(inList(filename,"stoplist.txt")) return 0;
#ifndef ESP32
  WiFiClient wificlient;
#endif
  HTTPClient http;
  http.setTimeout(10000);
#ifdef ESP32
  http.begin(url.c_str());
#else
  http.begin(wificlient,url.c_str());
#endif

  int code = http.GET();
  if(code != HTTP_CODE_OK) { http.end(); LOGI(LOG_UPGRADE,"Error-2: "+String(code)); return -2; }

  // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266HTTPClient/examples/StreamHttpClient/StreamHttpClient.ino

  if(is_file(filename+".tmp")) file_del(filename+".tmp");
  File file=SPIFFS.open(filename+".tmp","w"); if(!file) { http.end(); LOGI(LOG_UPGRADE, F("Error-3: file open")); return -3; }
  size_t size = http.getSize();
  if(size < 0) { http.end(); LOGI(LOG_UPGRADE, "Error-4: size: " + String(size)); return -4; }

  size_t uploaded = 0;
  #define lenbuf 1024
  uint8_t* duff = (byte*) malloc(lenbuf);

  WiFiClient *stream = http.getStreamPtr(); // get tcp stream

  LOGI(LOG_UPGRADE, "file_get_contents [" + String(filename) + "] size: " + String(size)+" FreeMemory: "+String(ESP.getFreeHeap()) );

  #define UPTIMEOUT 2000
  uint32_t lastms = millis();
  while( (millis()-lastms) < UPTIMEOUT && stream && http.connected() && ( size == -1 || uploaded < size )  ) {
	yield();
	size_t x = stream->available();
	if(!x) { delay(10); continue; } // чуть подождать, чтобы не зациклиться

	int c = stream->readBytes(duff, std::min(x, (size_t)lenbuf)); // std::min((size_t)x,(size_t)lenbuf) );
	if(c > 0) {

		LOGI(LOG_MAIN,"."); delay(200);

	    lastms = millis();
	    file.write(duff, c);
	    uploaded += c;
	} else {
	    LOGI(LOG_MAIN,"read timeout: "+String(c));
	    delay(200);
	}
  }

  if( size != -1 && uploaded >= size) LOGI(LOG_UPGRADE, "BREAK SIZE OK");
  if( (millis() - lastms) >= UPTIMEOUT ) LOGI(LOG_UPGRADE, "Error-timeout: " + String(millis()-lastms) + " millis=" + String(millis())+" lastms="+lastms);
  if( !http.connected() ) LOGI(LOG_UPGRADE, "Error: !http.connected()");

  file.close();
  http.end();
	    delay(200);
  free(duff);

  if(uploaded == size || size == -1) {
    if(md5 != "" && md5 != MD5file(filename + ".tmp")) {
      LOGI(LOG_UPGRADE, "Error-9: md5=["+md5+"]/["+MD5file(filename + ".tmp")+"] "+( is_file(filename+".tmp") ? "present" : "no file" ) );
      file_del(filename + ".tmp");
      return -9;
    }
    file_rename(filename+".tmp",filename);
    return 0;
  }

  LOGI(LOG_UPGRADE, "Error-8: size " + String(size) + " != uploaded " + String(uploaded));
  file_del(filename + ".tmp");
  return 8;
  // if(LOGLI(LOG_UPGRADE)) Serial.printf("\terror #%d: %s\n", c, http.errorToString(c).c_str()); }
}

String MD5file(String file) {
  file = Slash(file);
  if (!SPIFFS.exists(file)) return "";
  File f = SPIFFS.open(file, "r"); if (!f) return "";
  MD5Builder md5;
  md5.begin();
  md5.addStream(f, f.size()); // md5.add("Test333");
  md5.calculate();
  f.close();
  return md5.toString();
}


#include <Arduino.h>
#include "main.h"

#ifdef ESP32
    WebServer WEB ( DEFAULTPORT );
#else
    ESP8266WebServer WEB( DEFAULTPORT );
#endif

String UploadResult = "";
File fsUploadFile;

const char * headerkeys[] = {"User-Agent", "Cookie"} ;
size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);

void WEB_init() {
  esp_netif_init(); // йобаный патч иначе без WiFi на PlatformIO не работает
  WEB.collectHeaders(headerkeys, headerkeyssize );
  WEB.on("/upload", HTTP_POST, []() { otprav(UploadResult); }, handleFileUpload);
  WEB.on("/dir", HTTP_GET, WEB_DIR);
  WEB.on("/info", HTTP_GET, WEB_INFO);
  WEB.on("/FM", HTTP_GET, WEB_FM);
  WEB.on("/", HTTP_POST, WEB_MOTO_POST);
  WEB.on("/MOTO", HTTP_GET, WEB_MOTO);
  WEB.onNotFound(WEB_NOTFOUND);
  WEB.begin();
}

//===================================================
String web_find(String &s,String from,String to) { int i=0;
  if(from!="") { i=s.indexOf(from); if(i<0) return ""; i+=from.length(); }
  if(to=="") return s.substring(i);
  int j=s.indexOf(to,i); if(j<0) return "";
  int n=s.indexOf("\n",i); if(n>=0 && n<j) return "";
  return s.substring(i,j);
}

//===============================================================================
//===============================================================================
//===============================================================================
//===============================================================================

void WEB_MOTO_POST() { if(!AD(1)) return; web_moto(WEB.arg("plain")); }

void WEB_MOTO() {
  if (!AD(1) || !WEB.hasArg("MOTO") ) return;
  String s = WEB.arg("MOTO");
  s.replace("|", "\n");
  web_moto(s);
}

void web_moto(String s) {
  LOGI(LOG_WEB,"AJAX-MOTO: ["+s+"]");
  EchoMOTO=" "; MOTO(s,0); otprav(EchoMOTO.substring(1)); EchoMOTO="";
}

void WEB_DIR() { if(!AD(0)) return; otprav(REPER("{dir}")); }
void WEB_INFO() { if(!AD(0)) return; otprav(FullInfo()); }

//============================================
void WEB_FM() { String a=RE("a"); LOGI(LOG_WEB,"\nFM a="+a);
// разрешено всем

if(a=="login") { 
  // LOGI(LOG_WEB,"===> password: ["+CF("password")+"]");
  // LOGI(LOG_WEB,"===> passhash: ["+CF("passhash")+"]");
  // LOGI(LOG_WEB,"===> PASSWORD: ["+RE("password")+"]");
  // Cookie ESP=c4940ca950a6a8fe4cce1563fabbdce8

  if(CF("password")=="" || RE("password")!=CF("password")) otprav(""); otprav(CF("passhash")); }
if(a=="loop") { byte n=RE0("n"); if(n>=NLOOP || !Loopes[n].value) otprav(""); otprav(String(Loopes[n].value-Loopes[n].count)); return; }

if(a=="WIFIconn1") {
    LOGI(LOG_WEB,"========> Set network: ["+RE("net")+"] password: ["+RE("pass")+"]");
    if(CF("AP_name")!=HELP_NET_NAME) {
        LOGI(LOG_WEB,"========> Error: ["+CF("AP_name")+"] != [ESP-HELP]");
        return;
    }

MOTO(
"FILE.save.text /wifi_last.txt "+RE("net")+"\\n"+RE("pass")+"\n"
"sleep 1\n"
"ESP.reset\n"
);

}

return LOGI(LOG_WEB,"FM error: "+a);
}



//=======================================================
// Если не найден точный эндпоинт - значит, это какой-то файл
void WEB_NOTFOUND() { String s=String(WEB.uri());
    // if(WEB.hasHeader("Cookie")) LOGI(LOG_WEB,"\nFound cookie: ["+WEB.header("Cookie")+"] AD="+AD()+"\n");
  if(s.endsWith("/")) { s+="index.htm"; DOMOTO("play M"); }
  s=noSlash(s);
  if(inList("/"+s,"stopspam.txt")) return WEBNAH();

  // файлы админа с приставкой "admin_"
  if( AD() && ( filewebsend("admin_"+s) || filewebsend("admin_"+s+".gz") ) ) return;
  if( filewebsend(s) || filewebsend(s+".gz") ) return;

  if(s=="index.htm") {
    WEB.send(200,"text/html",DefaultINDEX);
    return;
  }

  if(ISWIFI) return WEBNAH();
  String l="http://"+WiFi.softAPIP().toString()+"/";
  LOGI(LOG_WEB,"Redirect: "+l);
  WEB.sendHeader("Location",l);
  WEB.send(302,"text/plain","Moved Permanently");
}


//======================================

String RE(String n) {
      if(!WEB.args()) return "";
      for(uint8_t i=0;i<WEB.args();i++) if(WEB.argName(i)==n) return WEB.arg(i);
      return "";
}
int RE0(String n) { return RE(n).toInt(); }

void WEBNAH() {
    WEB.sendHeader("Cache-Control","no-cache, no-store, must-revalidate");
    WEB.sendHeader("Pragma","no-cache");
    WEB.sendHeader("Expires","-1");
    WEB.send(404,"text/plain","404 Not Found");
}

struct ContentTypeMapping { const char *extension; const char *mimeType; };
const ContentTypeMapping mimeTable[] PROGMEM = {
  { ".htm",  "text/html; charset=utf-8" },
  { ".html", "text/html; charset=utf-8" },
  { ".svg",  "image/svg+xml" },
  { ".css",  "text/css" },
  { ".js",   "application/javascript" },
  { ".png",  "image/png" },
  { ".gif",  "image/gif" },
  { ".jpg",  "image/jpeg" },
  { ".ico",  "image/x-icon" },
  { ".xml",  "text/xml" },
  { ".pdf",  "application/x-pdf" },
  { ".zip",  "application/x-zip" },
  { ".gz",   "application/x-gzip" }
};
const int mimeTableSize = sizeof(mimeTable) / sizeof(mimeTable[0]);

String getContentType(String filename) {
  if(WEB.hasArg("download")) return F("application/octet-stream");
  for(int i=0; i<mimeTableSize; i++) {
    if(filename.endsWith(mimeTable[i].extension)) return FPSTR(mimeTable[i].mimeType);
  }
  return F("text/plain; charset=utf-8");
}



/*
String getContentType(String filename){
  if(WEB.hasArg("download")) return F("application/octet-stream");
  else if(filename.endsWith(".htm") || filename.endsWith(".html") ) return F("text/html; charset=utf-8");
  else if(filename.endsWith(".svg")) return F("image/svg+xml");
  else if(filename.endsWith(".css")) return F("text/css");
  else if(filename.endsWith(".js")) return F("application/javascript");
  else if(filename.endsWith(".png")) return F("image/png");
  else if(filename.endsWith(".gif")) return F("image/gif");
  else if(filename.endsWith(".jpg")) return F("image/jpeg");
  else if(filename.endsWith(".ico")) return F("image/x-icon");
  else if(filename.endsWith(".xml")) return F("text/xml");
  else if(filename.endsWith(".pdf")) return F("application/x-pdf");
  else if(filename.endsWith(".zip")) return F("application/x-zip");
  else if(filename.endsWith(".gz")) return F("application/x-gzip");
  return F("text/plain; charset=utf-8");
}
*/

void otprav(String s) {
      WEB.sendHeader(F("Access-Control-Allow-Origin"),"*");
      WEB.sendHeader(F("Content-Type"),F("text/plain; charset=utf-8"));
      WEB.send(200,F("text/plain"),s);
}


bool filewebsend(String f) { f=Slash(f);
    if(!SPIFFS.exists(f)) {
	    //LOGI(LOG_WEB,"WWW not found: "+f);
	return 0; }
    if( !AD() && inList(f,"stopweblist.txt") ) { LOGI(LOG_WEB, "WWW admin only: "+f); return 0; }
    File file = SPIFFS.open(f,"r"); if(!file) { LOGI(LOG_WEB,"WWW error: "+f); return 0; }
    size_t sent = WEB.streamFile(file,getContentType(f));
    file.close();
    LOGI(LOG_WEB,f+" -> "+String(sent));
    return 1;
}

bool AD() { return CF("passhash")=="" || WEB.header("Cookie").indexOf("ESP="+CF("passhash"))>=0 || RE("hash")==CF("passhash"); }
bool AD(bool i) { byte a=AD(); if(a) return a; otprav(i==0?"ERROR: Admin Only":"salert('You are not admin',3000);"); return 0; }

//=========================

void handleFileUpload() {
 if(!AD(1)) { UploadResult=F("ADMIN ONLY"); LOGI(LOG_WEB,F("NOT ADMIN")); return; }
 if(WEB.uri() != "/upload") return;

 HTTPUpload& upload = WEB.upload();

 if(upload.status == UPLOAD_FILE_START) { // 0
    UploadResult=F("START");
    String filename = Slash(upload.filename);
    LOGI(LOG_WEB,"START ["+filename+"]");
    if(filename=="/") { LOGI(LOG_WEB,F("EMPTY NAME")); return; } // нельзя пустое имя, оно в ESP32 ломает файловую систему
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();

 } else if(upload.status == UPLOAD_FILE_WRITE) {
    LOGI(LOG_WEB,".");
    if(fsUploadFile) fsUploadFile.write(upload.buf,upload.currentSize);

 } else if(upload.status == UPLOAD_FILE_END) {

    LOGI(LOG_WEB,"\nEND ["+WEB.uri()+"]totalSize: "+String(upload.totalSize)+" - ");
    if(fsUploadFile) { UploadResult=F("OK"); fsUploadFile.close(); }
    else UploadResult=F("ERR");
    LOGI(LOG_WEB,UploadResult);

 } else { UploadResult="ERROR"; LOGI(LOG_WEB,F("UPLOAD - UNKNOWN")); }
}

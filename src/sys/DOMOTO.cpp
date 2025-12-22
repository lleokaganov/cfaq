#include <Arduino.h>
#include "main.h"
#include "MODULES.h"

extern "C" {
  #include "esp_event.h"
  #include "esp_websocket_client.h"
  #include "esp_log.h"
}

String OKER(bool x) { return (x?F("OK"):F("ERROR")); }
String IOKER(bool x) { iecho (x?F("OK"):F("ERROR")); return F(""); }

String DOMOTO(String s,byte yild) {
  if(yild) yield();
  s=nocomments(s); s.replace("\t"," "); s.replace("\r",""); s.trim(); if(s=="") return EMPTY_STRING;
  String cmd=ARG(s,0);
  const char* cmd_c = cmd.c_str();  // ДО всех сравнений

  if(CMD_EQ("stop")
  || CMD_EQ("exit")
  || CMD_EQ("else")
  || CMD_EQ("repeat")
  || CMD_EQ("break")
  ) return cmd+" "+ARG_OTHER(s,0);

  if(CMD_EQ("ifrand")) { return String( random(0,PARG0(s,1)) ? "FALSE " : "TRUE ") + ARG_OTHER(s,1); }

  if(CMD_EQ("if")) {
    String z=PARG(s,2);
    String As=PARG(s,1);
    String Bs=PARG(s,3);
    double A=DPARG0(s,1);
    double B=DPARG0(s,3);
    bool IF;

    if( !A && !B && (z == "=" || z == "==" || z == "!=") ) { // текстовые константы
      if(z == "!=") IF=( As != Bs ? 1 : 0 );
      else IF = (As == Bs ? 1 : 0);
    }
    else if(z == "=" || z == "==") IF=( A == B ? 1 : 0);
    else if(z == "!=") IF=( A != B ? 1 : 0 );
    else if(z == ">") IF=(A > B ? 1 : 0 );
    else if(z == ">=") IF=(A >= B ? 1 : 0 );
    else if(z == "<") IF=(A < B ? 1 : 0 );
    else if(z == "<=") IF=(A <= B ? 1 : 0 );
    else return ERR(s,F("IF"));
    return String( IF ? "TRUE " : "FALSE " )+ARG_OTHER(s,3);
  }

  if(CMD_EQ("if.empty")) { return String( PARG(s,1) == "" ? "TRUE " : "FALSE ") + ARG_OTHER(s,1); }
  if(CMD_EQ("if.!empty")) { return String( PARG(s,1) != "" ? "TRUE " : "FALSE ") + ARG_OTHER(s,1); }

  if(CMD_EQ("ping")) { if(ISWIFI) { String url;
      uint16_t timeout=PARG0(s,1); if(!timeout) { timeout=2000; url=PARG_OTHER(s,0); } else url=PARG_OTHER(s,1); 
      MOTO(file_get_contents(url,timeout));
      } return EMPTY_STRING;
  }

  // if(CMD_EQ("attach")) { ServAttach( ARG(s,1).toInt(), ARG(s,2).toInt() ); return EMPTY_STRING; }
  // if(CMD_EQ("detach")) { ServDetach( ARG(s,1).toInt() ); return EMPTY_STRING; }
  // if(CMD_EQ("go")) { ServWrite( ARG(s,1).toInt(), ARG(s,2).toInt() ); return EMPTY_STRING;  }

    /// servo {pin} {angle} [{A1} {A2} {A3} {L}]
    /// 
  if(CMD_EQ("servo")) {
    int pin = PARG(s,1).toInt();
    int angle = PARG(s,2).toInt();
    int A1 = PARG(s,3).toInt(); if(!A1) A1=180;
    int A2 = PARG(s,4).toInt(); if(!A2) A2=1000;
    int A3 = PARG(s,5).toInt(); if(!A3) A3=2000;
    int L = PARG(s,6).toInt(); if(!L) L=50;
    int pulseWidth = map(angle, 0, A1, A2, A3); // Преобразуем угол в длительность импульса (в мкс)
    // if(pulseWidth <= 0) { digitalWrite(pin, LOW); delayMicroseconds(20000); }
    // else if(pulseWidth >= 20000) { digitalWrite(pin, HIGH); delayMicroseconds(20000); }
    // else {    
    for (int i = 0; i < L; i++) {                   // Генерируем сигнал с частотой 50 Гц (20 мс)
      digitalWrite(pin, HIGH);                       // Включаем пин
      delayMicroseconds(pulseWidth);                 // Ждём нужное время (длительность импульса)
      yield();
      digitalWrite(pin, LOW);                        // Выключаем пин
      delayMicroseconds(20000 - pulseWidth);         // Ждём оставшееся время до 20 мс
      yield();
    }
    // }
    return EMPTY_STRING;
  }

  if(CMD_EQ("pinmode")) {
      String A=PARG(s,1); A.replace("gpio",""); // A.replace("pin","");
#ifdef ESP32
      uint8_t pin = A.toInt();
#else
      uint8_t pin = (A=="A0" ? A0 : A.toInt() );
#endif
      String mod=PARG(s,2);

      if(mod=="INPUT_PULLUP") pinMode(pin,INPUT_PULLUP);
      // else if(mod=="ANALOG_OUTPUT") pinMode(pin,ANALOG_OUTPUT);
      // else if(mod=="INPUT_PULLDOWN16") pinMode(pin,INPUT_PULLDOWN16); нету в ESP8266
      else if(mod=="INPUT") pinMode(pin,INPUT);
#ifdef ESP32
#else
      else if(mod=="SPECIAL") pinMode(pin,SPECIAL);  // ??? непонятно, что, но работает
#endif
      else pinMode(pin,OUTPUT);
      return EMPTY_STRING;
  }

  if(CMD_EQ("blink")) { String pin=PARG(s,1); byte p=(pin==""?CF("blink.pin","2"):pin).toInt(); digitalWrite(p,!digitalRead(p)); return EMPTY_STRING; }

  if(CMD_EQ("pin")) { digitalWrite(PARG0(s,1),PARG0(s,2)); return EMPTY_STRING; } //http://wikihandbk.com/wiki/ESP8266:%D0%9F%D1%80%D0%BE%D1%88%D0%B8%D0%B2%D0%BA%D0%B8/Arduino/%D0%A1%D0%BF%D1%80%D0%B0%D0%B2%D0%BE%D1%87%D0%BD%D0%B8%D0%BA_%D0%BF%D0%BE_%D1%84%D1%83%D0%BD%D0%BA%D1%86%D0%B8%D1%8F%D0%BC_%D0%B0%D0%B4%D0%B4%D0%BE%D0%BD%D0%B0_ESP8266_%D0%B4%D0%BB%D1%8F_IDE_Arduino
  if(CMD_EQ("pwm")) { analogWrite(PARG0(s,1),PARG0(s,2)); return EMPTY_STRING; }
  if(CMD_EQ("tone")) { uint16_t dur=PARG0(s,3); if(dur) tone(PARG0(s,1),PARG0(s,2),dur); else tone(PARG0(s,1),PARG0(s,2)); return EMPTY_STRING; }
  if(CMD_EQ("play")) { byte pin=PARG0(s,1); if(!pin) play(ARG_OTHER(s,0)); else play(ARG_OTHER(s,1),pin); return EMPTY_STRING; }
#ifdef ESP32
 /////////////////////////////////////////// if(CMD_EQ("dac")) { dacWrite(PARG0(s,1),PARG0(s,2)); return EMPTY_STRING; } // 255= 3.3V, 128=1.65V
#endif
  
  if(CMD_EQ("playip")) {
      static char nd[]="MLd";
      String ipn=ARG(WiFi.localIP().toString(),3,".");
      String s=""; for(int i=0;i<3;i++) s+=(ipn[i]=='0' ? "play .6c. ; " : "repeat "+String(ipn[i])+" play ,4"+String(nd[i])+" ; play .. ; ");
      MOTO(s);
      return EMPTY_STRING;
  }

  if(CMD_EQ("delay") || CMD_EQ("usleep")) { delay(PARG0(s,1)); return EMPTY_STRING; }
  if(CMD_EQ("sleep")) { delay(PARG0(s,1)*1000); return EMPTY_STRING; }

  if(CMD_EQ("run")) { MOTO(getfile(PARG(s,1))); return EMPTY_STRING; }
  if(CMD_EQ("do")) { MOTO(PARG_OTHER(s,0)); return EMPTY_STRING; }

  if(CMD_EQ("post")) { return "POST "+ARG_OTHER(s,0); }
 
  if(CMD_EQ("loopfile")) { return "LOOPFILE "+ARG_OTHER(s,0); }

  if(CMD_EQ("at")) { return "LOOP 1 "+ARG_OTHER(s,0); } // at 10 /START    at 1 10 /START
  if(CMD_EQ("loops")) { return "LOOP "+ARG_OTHER(s,0); }
  if(CMD_EQ("loop")) { return "LOOP 0 "+ARG_OTHER(s,0); }
  if(CMD_EQ("loop.del")) { uint8_t cikl=PARG0(s,1); loop_del(cikl); return EMPTY_STRING; }
  if(CMD_EQ("now")) { loop_now(ARG_OTHER(s,0)); return EMPTY_STRING; } // at 0
  
if(CMD_EQ("set")) { return "set "+PARG(s,1)+" "+PARG_OTHER(s,(PARG(s,2)=="="?2:1)); }

  String x = ARG(s,1); if(x == "=" || ( x.length()==2 && x[1] == '=' )) {
    String imq=cmd; imq.replace("{",""); imq.replace("}",""); // Z = 1 и {Z} = 1 одинаково допустимы
    double acc=0;
    uint16_t k=2;
    if(x == "+=" || x == "-=" || x == "/=" || x == "*=") { acc=DCF0(imq); x.replace("=",""); }
    
    while(k < count(s)) {
        double ARG2=DPARG0(s,k++);
        if(x=="/") acc = acc / ARG2;
        else if(x=="*") acc = acc * ARG2;
        else if(x=="+") acc = acc + ARG2;
        else if(x=="-") acc = acc - ARG2;
        else if(x=="%") acc = (int)acc % (int)ARG2;
        else if(x=="=") acc = ARG2;
        else return "ERR let("+String(k)+") "+s;
        x=PARG(s,k++);
    }

    x=String(acc);
    if(x.endsWith(".00")) x=x.substring(0,x.length()-3);

    while(k < count(s)) {
        double ARG2=DPARG0(s,k++);
        if(x=="/") acc = acc / ARG2;
        else if(x=="*") acc = acc * ARG2;
        else if(x=="+") acc = acc + ARG2;
        else if(x=="-") acc = acc - ARG2;
        else if(x=="=") acc = ARG2;
        else return "ERR let("+String(k)+") "+s;
        x=PARG(s,k++);
    }
    
    x=String(acc);
    if(x.endsWith(".00")) x=x.substring(0,x.length()-3);
      // if(imq == "gpio16reset") gpio16reset=x.toInt();
      // else 
	if(1) {}
    #ifdef USE_FLT
      else if(imq == "MaxA0") { MaxA0=x.toInt(); return EMPTY_STRING; } // максимальный порог >= A0 для срабатывания MaxA0.callback
      else if(imq == "MinA0") { MinA0=x.toInt(); return EMPTY_STRING; }  // минимальный порог < A0 для срабатывания MinA0.callback
      else if(imq == "FltA0") { FltA0=x.toInt(); return EMPTY_STRING; }  // результат фильтра A0 FLT для срабатывания FltA0.callback
    #endif

    #ifdef USE_ENCODER
      else if(imq == "ENCODER") { ENCODER=x.toInt(); return EMPTY_STRING; }
    #endif

      CFSET(imq,x);
      return EMPTY_STRING;
  } else if(x == "~") {
       String imq=cmd; imq.replace("{",""); imq.replace("}","");
       uint16_t acc= ~ PARG0(s,2);
       CFSET(imq,String(acc));
       return EMPTY_STRING;
  } else if(x == "unsigned") {
       String imq=cmd; imq.replace("{",""); imq.replace("}","");
       uint16_t acc=PARG0(s,2);
       CFSET(imq,String(acc));
       return EMPTY_STRING;
  }


// ======================================================================================

  if(CMD_EQ("WIFI")) {
    String login=PARG(s,1);
    String password=PARG(s,2);
    if(login=="") {
      login = "lleo";
      password = "testpassword";
    }
    LOG("WIFI try to connect '"+login+"' password: '"+password+"'");
    WiFi.begin(login.c_str(),password.c_str());

    // WiFi.begin(ssid, password, channel, bssid, connect)
  return EMPTY_STRING; }

  if(CMD_EQ("WIFI.onevent")) {
    #ifdef ESP32
    WiFi.onEvent(WiFiEvent);
    #endif
    return EMPTY_STRING;
    }

  if(CMD_EQ("WIFI.reconnect")) { WiFi.reconnect(); return EMPTY_STRING; } // Чип отключается от точки доступа, а затем инициирует с нею повторное подключение.
  if(CMD_EQ("WIFI.disconnect")) { WiFi.disconnect(PARGB(s,1,true)); return EMPTY_STRING; } // Выставляет SSID и пароль на null, отключает станцию от точки доступа. Если true, это выключит режим станции.
  if(CMD_EQ("WIFI.APdisconnect")) { WiFi.softAPdisconnect(PARGB(s,1,true)); return EMPTY_STRING; } // Выставляет SSID и пароль на null, отключает станцию от точки доступа. Если true, это выключит режим станции.

  if(CMD_EQ("if.WIFI") || CMD_EQ("if.!WIFI")) {
    byte i=(ISWIFI?1:0); i=(CMD_EQ("if.WIFI")?i:!i);
    return String(i?"TRUE ":"FALSE ")+ARG_OTHER(s,0);
  }

  if(CMD_EQ("WEB.begin")) { webstart=1; WEB.begin(); return EMPTY_STRING; }
  if(CMD_EQ("WEB.close")) { webstart=0; WEB.close(); return EMPTY_STRING; }
  if(CMD_EQ("WEB.stop")) { webstart=0; WEB.stop(); return EMPTY_STRING; }

  if(CMD_EQ("WIFI.persistent")) { WiFi.persistent(PARGB(s,1,false)); return EMPTY_STRING; } // сохранять ли на флеш-память каждый раз пароль

  if(cmd == "WIFI.autoconnect") {
    #if defined(ESP32)
        WiFi.setAutoReconnect(PARGB(s, 1, true));
    #elif defined(ESP8266)
        WiFi.setAutoConnect(PARGB(s, 1, true));
    #else
        #error "Unknown platform! Add support for your chip."
    #endif
    return EMPTY_STRING;
  }  // Чтобы при включении питания автоматически подключался к последней использованной точке доступа. Если false, автоматическое подключение будет деактивировано, иначе активировано
  
  if(cmd == "if.WIFI.autoconnect") {
    #if defined(ESP32)
        bool isAutoConnect = WiFi.getAutoReconnect();
    #elif defined(ESP8266)
        bool isAutoConnect = WiFi.getAutoConnect();
    #endif
    return String(isAutoConnect ? "TRUE " : "FALSE ") + ARG_OTHER(s, 0);
  } // возвращает true, если у модуля активировано автоматическое подключение

  if(cmd == "WIFI.autoreconnect") { 
    #if defined(ESP32)
        WiFi.setAutoReconnect(PARGB(s, 1, true));
    #elif defined(ESP8266)
        WiFi.setAutoConnect(PARGB(s, 1, true)); // Аналогично `setAutoReconnect`
    #endif
    return EMPTY_STRING; 
  } // Если параметр autoReconnect выставлен на true, модуль сделает повторное подключение, а если false, то нет.

  if(CMD_EQ("WIFI.waitconnect")) { // Ждет, когда модуль подключится к точке доступа.
    uint32_t start=UnixTime;
    while( UnixTime < start+20 && WiFi.status()!=WL_CONNECTED ) { delay(200); LOG(".",1); }
    // WiFi.waitForConnectResult();
    return EMPTY_STRING;
   }

//  if(CMD_EQ("WIFI.useStaticBuffers")) { WiFi.useStaticBuffers(PARGB(s,1,false)); return EMPTY_STRING; } // WIFI.useStaticBuffers YES - учлучшить быстродействие за счет памяти

  if(CMD_EQ("WIFI.diag")) { WiFi.printDiag(Serial); return EMPTY_STRING; }

  if(CMD_EQ("WIFI.hostname")) { WiFi.hostname(PARG(s,1)); return EMPTY_STRING; }
  // if(CMD_EQ("WIFI.APhostname")) { WiFi.softAPsetHostname(PARG(s,1)); return EMPTY_STRING; }

  if(CMD_EQ("WIFI.mode")) { String c=PARG(s,1); LOG(cmd+" ["+PARG(s,1)+"]");
      // WifiMode = 0; // состояние Wifi: 0 - не инициализировано, 1 - STA, 2 - AP, 3 - AP_STA
      byte i=WifiMode;
      if(c=="STA") { WiFi.mode(WIFI_STA); i=1; }
      else if(c=="AP") { WiFi.mode(WIFI_AP); i=2; }
      else if(c=="AP_STA") { WiFi.mode(WIFI_AP_STA); i=3; }
      else return EMPTY_STRING;
      if(!WifiMode && i) { WEB.begin(); webstart=1; LOG("WEB server started"); } // йобаный патч, если впервые подключились
      WifiMode=i;
      return EMPTY_STRING;
  }

  if(CMD_EQ("WIFI.config")) {
    IPAddress ip; ip.fromString(PARG(s,1));
    IPAddress gw; gw.fromString(PARG(s,2));
    IPAddress msk; msk.fromString(PARG(s,3));
    if(PARG(s,4)=="") { WiFi.config(ip,gw,msk); return EMPTY_STRING; }
    IPAddress dns1; dns1.fromString(PARG(s,4));
    IPAddress dns2; dns2.fromString(PARG(s,5));
    WiFi.config(ip,gw,msk,dns1,dns2); return EMPTY_STRING;
  }

  if(CMD_EQ("WIFI.AP")) {
      // поднять точку доступа WIFI.AP логин пароль [канал] [hidden] [max_connections]
      // поднять точку доступа WIFI.AP логин пароль 6 0 10
      String login=PARG(s,1);
      String pass=PARG(s,2);
      String chan=PARG(s,3);
      String hidden=PARG(s,4);
      String max_connection=PARG(s,5); 
      bool res=WiFi.softAP( login.c_str(),
                   (pass==""||pass=="NULL"? NULL : pass.c_str() ),
                   (chan==""||chan=="NULL"? 6 : chan.toInt() ),
                   (hidden==""||hidden=="NULL"? 0 : hidden.toInt() ),
                   (max_connection==""||max_connection=="NULL"?4:max_connection.toInt())
                   ); //, bool ftm_responder = false);
// ftm_responder sets the Wi-Fi FTM responder feature. Only for ESP32-S2 and ESP32-C3 SoC!     
      LOG(" AP login=["+login+"] pass=["+pass+"] configured: "+OKER(res) );
    return EMPTY_STRING;
  }

  if(CMD_EQ("WIFI.APconfig")) { // настроить точку доступа softAPConfig (local_ip, gateway, subnet)
    IPAddress ip; ip.fromString(PARG(s,1));
    IPAddress gw; gw.fromString(PARG(s,2));
    IPAddress msk; msk.fromString(PARG(s,3));
    WiFi.softAPConfig(ip,gw,msk);
    return EMPTY_STRING;
  }
  
  if(CMD_EQ("WiFi.scanDelete")) { WiFi.scanDelete(); return EMPTY_STRING; } // очистить память после сканирования WiFi
  if(CMD_EQ("WiFi.scanNetworks")) { WiFi.scanNetworks(true,PARGB(s,1,false)); return EMPTY_STRING; }

// =======================================================================================================

  if(CMD_EQ("echo")) { 
    s.replace("\\n","\n");
    iecho(REPER(ARG_OTHER(s,0)));
    return EMPTY_STRING;
  }

  if(CMD_EQ("echomoto.init")) { EchoMOTO = " "; return EMPTY_STRING; }

  if(CMD_EQ("echo.buf")||CMD_EQ("echobuf")) {
      if(Buf==NULL) return EMPTY_STRING;
      uint16_t x=PARG0(s,1) & (NBUF-1);
      while(x != bufn) {
        iecho("["+String(Buf[x].A0)+","+String(Buf[x].kn)+","+x+"],");
        x = (++x) & (NBUF-1);
      }
      return EMPTY_STRING;
  }
        
  if(CMD_EQ("TIMER.start")||CMD_EQ("timstart")) { // TIMER.start ([millisecond=50]) (NBUF=16,32,64,128,256,512)
    
    if(Buf == NULL) {
      uint16_t x=PARG0(s,2); if(!x) x = 64;
      if(x!=16 && x!=32 && x!= 64 && x!=128 && x!=256 && x!= 512) return ERR(s,F("NBUF size"));
      NBUF = x;
        if(CF("FLT")=="") FLT_init(); else DOMOTO("set.FLT "+CF("FLT"),yild);
        FLT_c=0;
        Buf = (BufLoopes *) calloc(NBUF, sizeof(BufLoopes));
        // Serial.println("BUF: Free heap="+String(ESP.getFreeHeap(), DEC));
       
        for(uint16_t i=0;i<NBUF;i++) { Buf[i].kn=0; Buf[i].A0=0; } bufn=NBUF-1;
        float ms=PARG0(s,1); if(!ms) ms=50;
        timpin.attach((ms>=1?ms/1000:ms),pinlog_engine); //0.05
    }
    return EMPTY_STRING;
  }
       
  if(CMD_EQ("TIMER.stop")||CMD_EQ("timstop")) {
      if(Buf != NULL) { timpin.detach(); free(Buf); Buf = NULL; }
      return EMPTY_STRING;
   }

  if(CMD_EQ("set.FLT") || CMD_EQ("setflt") ) { // String l;
    int l = ARG(s,1).toInt(); if(l >= FLT_LAG_MAX) return ERR( s, String(F("lag >= "))+String(FLT_LAG_MAX) ); FLT_lag = l;
    FLT_TOL = ARG(s,2).toFloat();
    FLT_THRESHOLD = ARG(s,3).toFloat();
    FLT_INFLUENCE = ARG(s,4).toFloat();
    return EMPTY_STRING;
  }

  if(CMD_EQ("set.FLT.TYPE") ) {
    String l = ARG(s,1);
    if(l == "chuk") FLT_type = 0;
    else if(l == "mean") FLT_type = 1;
    else return ERR(s);
    return EMPTY_STRING;
  }

  if(CMD_EQ("if.FILE")) { return String( is_file(PARG(s,1)) ? "TRUE " : "FALSE ") + ARG_OTHER(s,1); }
  if(CMD_EQ("if.!FILE")) { return String( is_file(PARG(s,1)) ? "FALSE " : "TRUE ") + ARG_OTHER(s,1); }

  if(CMD_EQ("FILE.save") || CMD_EQ("savefile")) { return "FSAVE "+PARG_OTHER(s,0); }
  if(CMD_EQ("FILE.add") || CMD_EQ("addfile")) { return "FSAVE+ "+PARG_OTHER(s,0); }
  if(CMD_EQ("FILE.add.ln") || CMD_EQ("addfileln")) { return "FSAVE+ "+PARG_OTHER(s,0)+"\n"; }
  // с заменой \\n на \n
  if(CMD_EQ("FILE.save.text") || CMD_EQ("savetextfile")) { return "FTSAVE "+PARG_OTHER(s,0); }
  if(CMD_EQ("FILE.add.text") || CMD_EQ("addtextfile")) { return "FTSAVE+ "+PARG_OTHER(s,0); }

  if(CMD_EQ("FILE.del")) return IOKER( file_del(PARG(s,1)) );
  if(CMD_EQ("FILE.rename")) return IOKER( file_rename(PARG(s,1),PARG(s,2)) );
  if(CMD_EQ("FILE.copy")) return IOKER( file_copy(PARG(s,1),PARG(s,2)) );

  if(CMD_EQ("FILE.string.del") || CMD_EQ("delstring")) { // удалить из файла строку если такая была
    String f=PARG(s,1); String code=PARG_OTHER(s,1); if(f=="") return ERR(s); String S=getfile(f);
    uint16_t l=findstring(S,code);
    if(l!=0xFFFF) return IOKER(  file_save(f, S.substring(0,l)+ARG_OTHER(S.substring(l),0,"\n"))  );
    iecho("OK");
    return EMPTY_STRING;
  }

  if(CMD_EQ("FILE.string.add") || CMD_EQ("addstring")) { // добавить в файл строку, если такой не было
    String f=PARG(s,1); String code=PARG_OTHER(s,1); if(f=="") return ERR(s); String S=getfile(f);
    if( findstring(S,code)==0xFFFF ) return "FSAVE+ "+ARG_OTHER(s,0)+"\n"; else { iecho("OK1"); return EMPTY_STRING; }
  }

  if(CMD_EQ("FILE.key.del")) { // [delkey /cards-close.txt 883493A887]
    String file=PARG(s,1); String key=PARG(s,2); if(file==""||key=="") return ERR(s);
    String S=getfile(file); uint16_t l=findkey(S,key);
    if(l!=0xFFFF) return IOKER( file_save(file, S.substring(0,l)+ARG_OTHER(S.substring(l),0,"\n"))  );
    return F("");
  }

  if(CMD_EQ("FILE.key.add")) { // добавить или изменить
    String file=PARG(s,1); String key=PARG(s,2); String value=PARG_OTHER(s,2);
    if(file==""||key=="") return ERR(s);
    String S=getfile(file); uint16_t l=findkey(S,key);
    if(l!=0xFFFF) S=S.substring(0,l)+key+" "+value+"\n"+ARG_OTHER(S.substring(l),0,"\n"); // заменить
    else S=S+key+" "+value+"\n"; // иначе просто добавить
    return IOKER( file_save(file,S) ?"OK":"ERROR");
    return F("");
  }

  if(CMD_EQ("FILE.upload")) { // загрузить файл по сети FILE.upload url filename
    String url=PARG(s,1); String filename=PARG(s,2);
    if(url==""||filename=="") return ERR(s);
    int8_t res=file_upload_binary(url,filename);
    LOG("Result: "+String(res));
    return EMPTY_STRING;
  }

  if(CMD_EQ("LOG")) {
    String l=PARG(s,1);
    uint16_t b = (  l=="ALL" ? 0xFFFF : (1<<LOG_NAME_I(l)) );
        LOGI(LOG_MAIN,"\t\tLOG SET: "+s+" ["+String(b)+"]");
        // Serial.println("\tSET: ["+s+"]=["+String(b)+"] LOG_NAME_I("+l+")="+String(LOG_NAME_I(l)) );
    LOGISET = ( PARGB(s,2,1) ? LOGISET | b : LOGISET & ~b );
    return EMPTY_STRING;
  }

#ifndef ESP32
  // --- serial ---
  // Serial.begin(9600, SERIAL_8N1); //GPIO1 (TX) and GPIO3 (RX), 9600kbps, 8-bit data, no parity, 1-bit stop
  // Serial.swap(); //GPIO15 (TX) and GPIO13 (RX)
  // Serial.swap(); //swap back to GPIO1 (TX) and GPIO3 (RX)
  if(CMD_EQ("SERIAL.swap")) { //swap back to GPIO1 (TX) and GPIO3 (RX) -> GPIO15 (TX) and GPIO13 (RX)
      if(PARG(s,1)=="" || PARG0(s,1) != SerialSwap ) { Serial.swap(); SerialSwap=!SerialSwap; }  
      return EMPTY_STRING;
  }

  if(CMD_EQ("SERIAL1.flush")) { Serial1.flush(); return EMPTY_STRING; }
  if(CMD_EQ("SERIAL1.end")) { LOGI(LOG_MAIN,cmd); Serial1.end(); return EMPTY_STRING; }
  if(CMD_EQ("SERIAL1.begin")) { // SERIAL.begin 115200 RX
    unsigned long spd = strtoul(PARG(s,1).c_str(),NULL,0); if(!spd) spd=115200;
    LOGI(LOG_MAIN,cmd+" "+String(spd));
    Serial1.begin(spd); return EMPTY_STRING;
  } //   Serial1.begin(115200, SERIAL_8N1, 18, 17); // ??????????

  if(CMD_EQ("SERIAL1.setTimeout")) { uint32_t t = PARG0(s,1); LOGI(LOG_MAIN,cmd+" "+String(t)); Serial1.setTimeout(t); return EMPTY_STRING; }
#endif

  if(CMD_EQ("SERIAL.setTimeout")) { uint32_t t = PARG0(s,1); LOGI(LOG_MAIN,cmd+" "+String(t)); Serial.setTimeout(t); return EMPTY_STRING; }
  if(CMD_EQ("SERIAL.flush")) { Serial.flush(); return EMPTY_STRING; }
  if(CMD_EQ("SERIAL.end")) { LOGI(LOG_MAIN,cmd); Serial.end(); return EMPTY_STRING; }
  if(CMD_EQ("SERIAL.begin")) { // SERIAL.begin 115200 RX
    unsigned long spd = strtoul(PARG(s,1).c_str(),NULL,0); if(!spd) spd=115200;
    LOGI(LOG_MAIN,cmd+" "+String(spd));
    Serial.begin(spd);
    return EMPTY_STRING;

    // https://github.com/plerup/espsoftwareserial/blob/main/src/SoftwareSerial.h
    // https://arduino-esp8266.readthedocs.io/en/latest/reference.html?highlight=Serial#serial
  }


// --- interrupt ---
  if(CMD_EQ("INTERRUPT.add")) { // pin mode prog
    byte pin = PARG0(s,1); uint16_t ipin = digitalPinToInterrupt(pin);
    String mod = PARG(s,2);
    byte m;
    if(mod == "1"||mod=="RISING") m = RISING;
    else if(mod == "0"||mod=="FALLING") m = FALLING;
    else if(mod == "2"||mod=="CHANGE") m = CHANGE;
    else return ERR(s,F("Unknown mode"));
    CFSET("INT_"+String(pin),""); // PARG_OTHER(s,2));
          if(pin==0) attachInterrupt(ipin,INTERRUPT_0,m);
    else  if(pin==1) attachInterrupt(ipin,INTERRUPT_1,m);
    else  if(pin==2) attachInterrupt(ipin,INTERRUPT_2,m);
    else  if(pin==3) attachInterrupt(ipin,INTERRUPT_3,m);
    else  if(pin==4) attachInterrupt(ipin,INTERRUPT_4,m);
    else  if(pin==5) attachInterrupt(ipin,INTERRUPT_5,m);
    else if(pin==12) attachInterrupt(ipin,INTERRUPT_6,m);
    else if(pin==13) attachInterrupt(ipin,INTERRUPT_7,m);
    else if(pin==14) attachInterrupt(ipin,INTERRUPT_8,m);
    else if(pin==15) attachInterrupt(ipin,INTERRUPT_9,m);
#ifdef ESP32
    else if(pin==16) attachInterrupt(ipin,INTERRUPT_10,m);
    else if(pin==17) attachInterrupt(ipin,INTERRUPT_11,m);
    else if(pin==18) attachInterrupt(ipin,INTERRUPT_12,m);
    else if(pin==19) attachInterrupt(ipin,INTERRUPT_13,m);

    else if(pin==21) attachInterrupt(ipin,INTERRUPT_14,m);
    else if(pin==22) attachInterrupt(ipin,INTERRUPT_15,m);
    else if(pin==23) attachInterrupt(ipin,INTERRUPT_16,m);

    else if(pin==25) attachInterrupt(ipin,INTERRUPT_17,m);
    else if(pin==26) attachInterrupt(ipin,INTERRUPT_18,m);
    else if(pin==27) attachInterrupt(ipin,INTERRUPT_19,m);

    else if(pin==32) attachInterrupt(ipin,INTERRUPT_20,m);
    else if(pin==33) attachInterrupt(ipin,INTERRUPT_21,m);
    else if(pin==34) attachInterrupt(ipin,INTERRUPT_22,m);
    else if(pin==35) attachInterrupt(ipin,INTERRUPT_23,m);
    else if(pin==36) attachInterrupt(ipin,INTERRUPT_24,m);

    else if(pin==39) attachInterrupt(ipin,INTERRUPT_25,m);
#endif
    else {
      LOGI(LOG_ERROR,cmd+" "+String(pin));
      return EMPTY_STRING;
    }

    LOGI(LOG_MAIN,cmd+" "+String(pin)+" mode="+String(m));
    return "set INT_"+String(pin)+" "+PARG_OTHER(s,2);
  }

  if(CMD_EQ("INTERRUPT.del")) { detachInterrupt(digitalPinToInterrupt(PARG0(s,1))); return EMPTY_STRING; }
// --- interrupt ---


// === LAN ===

if(CMD_EQ("ESP.deepSleep")) { // deepSleep microsecund
#ifdef ESP32
  ESP.deepSleep(PARG0(s,1));
#else
  // Контакт GPIO16 должен быть привязан к RST – чтобы вывести чип из режима глубокого сна.
  String o=PARG(s,2); RFMode mode;
  if(o=="WAKE_RF_DEFAULT") mode=WAKE_RF_DEFAULT;
  else if(o=="WAKE_RFCAL") mode=WAKE_RFCAL;
  else if(o=="WAKE_NO_RFCAL") mode=WAKE_NO_RFCAL;
  else if(o=="WAKE_RF_DISABLED") mode=WAKE_RF_DISABLED;
  else { LOG(cmd+" wrong mode"); return EMPTY_STRING; } 
  ESP.deepSleep(PARG0(s,1),mode);
#endif
  return EMPTY_STRING;
}

// if(CMD_EQ("ADC_MODE")) { ADC_MODE(ADC_VCC); return EMPTY_STRING; } // в самом начале
if(CMD_EQ("ESP.restart")) { ESP.restart(); return EMPTY_STRING; }
if(CMD_EQ("ESP.reset")) { 
#ifdef ESP32
  ESP.restart();
#else
  ESP.reset();
#endif
return EMPTY_STRING; }

if(CMD_EQ("SPIFFS.format")) {
  String wifi=getfile("/wifi_last.txt");
  LOGI(LOG_MAIN, F("Format SPIFFS")); SPIFFS.format(); LOGI(LOG_MAIN, F("Format SPIFFS - OK"));
  file_save("/wifi_last.txt",wifi);
  ESP.restart();
}

if(CMD_EQ("Breath.set")) { // Установка параметров Breath: Breath.set 180000 60000000 2 600 500 1200 1 // Breath.set 280000 60000000 2 600 500 1100 1
  Breath.TT=(uint32_t)PARG0(s,1); // интервал 1
  Breath.TTT=(uint32_t)PARG0(s,2); // интервал 2
  Breath.pinok=(uint8_t)PARG0(s,3); // pin
  pinMode(Breath.pinok,OUTPUT);
  Breath.Z=(uint16_t)PARG0(s,4);   // исходное значение
  Breath.min=(uint16_t)PARG0(s,5); // min
  Breath.max=(uint16_t)PARG0(s,6); // max
  Breath.K=(int16_t)PARG0(s,7); // ШАГ
  return EMPTY_STRING;
}

if(CMD_EQ("SHEDULE.start")) { SheduleS=getfile("/Shedule.txt"); if(SheduleS=="") SheduleInit=0; else { SheduleS="\n"+SheduleS; SheduleInit=1; } return EMPTY_STRING; }
if(CMD_EQ("SHEDULE.stop")) { SheduleS=""; SheduleInit=0; return EMPTY_STRING; }

#ifdef ESP32

  if(CMD_EQ("analogReadResolution")) {
    //  9 → 0..511
    // 10 → 0..1023
    // 11 → 0..2047
    // 12 → 0..4095 (дефолт)
    String x = PARG(s,1);
    uint8_t n = 12;
    if(x == "9" || x =="512") n = 9;
    else if(x == "10" || x =="1024") n = 10;
    else if(x == "11" || x =="2048") n = 11;
    analogReadResolution(n);
    return EMPTY_STRING;
  }

  if(CMD_EQ("analogWriteResolution")) {
    //  9 → 0..511
    // 10 → 0..1023
    // 11 → 0..2047
    // 12 → 0..4095 (дефолт)
    String x = PARG(s,1);
    uint8_t n = 12;
    if(x == "9" || x =="512") n = 9;
    else if(x == "10" || x =="1024") n = 10;
    else if(x == "11" || x =="2048") n = 11;
    analogWriteResolution(n);
    return EMPTY_STRING;
  }

  if(CMD_EQ("analogSetAttenuation")) {
        // 	Примерный диапазон
        // ADC_0db	0 – ~1.1 V
        // ADC_2_5db	0 – ~1.5 V
        // ADC_6db	0 – ~2.2 V
        // ADC_11db	0 – ~3.3 V
    String v = PARG(s,1);
    String arg2 = PARG(s,2);
    int pin = -1;
    if(arg2 != "") {
      pin = v.toInt();
      v = arg2;
    }
    adc_attenuation_t x = ADC_ATTENDB_MAX;
    if(v=="ADC_0db" || v=="1.1") x = ADC_0db;
    else if(v=="ADC_2_5db" || v=="1.5") x = ADC_2_5db;
    else if(v=="ADC_6db" || v=="2.2") x = ADC_6db;
    else if(v=="ADC_11db" || v=="3.3") x = ADC_11db;
    if(pin < 0) {
        analogSetAttenuation(x);
    } else {
        analogSetPinAttenuation(pin, x);
    }
    return EMPTY_STRING;
  }

  if(CMD_EQ("adcAttachPin")) {
    uint8_t pin = PARG0(s,1);
    adcAttachPin(pin);
    return EMPTY_STRING;
  }

  if(CMD_EQ("analogSetClockDiv")) {
    // Делитель тактовой частоты АЦП (АЦП шумный, медленнее = стабильнее) значения: 1…255, дефолт: 1
    int div = PARG0(s,1);
    analogSetClockDiv(div);
    return EMPTY_STRING;
  }

#endif







  #include "MODULES_domoto.cpp"

  // for (const ModEntry* e = __start_domoto; e < __stop_domoto; e++) {
  //   if (cmd == e->name) {
  //       e->fn(s);
  //       return EMPTY_STRING;
  //   }
  // }

  String f=getfile(cmd); if(f!="") MOTO(f,yild);
  else return ERR(s,F("error command"));
  return EMPTY_STRING;
}
String DOMOTO(String s) { return DOMOTO(s,1); }

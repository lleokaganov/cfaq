#include <Arduino.h>
#include "main.h"

LoopStruct Loopes[NLOOP];
Setting Settes[NSET];

byte webstart=WEB_START_DEFAULT; // Обслуживать ли веб-сервер
byte SheduleInit=SHEDULE_START_DEFAULT; // Планировщик запущен

#include "MODULES_main.cpp"

byte SerialSwap = 0; // состояние переключения Serial

String EchoMOTO = ""; // строка для вывода инфы в Аякс

// === interrupts ===
  volatile uint32_t InterruptsFlags=0;
  void ICACHE_RAM_ATTR INTERRUPT_0() { InterruptsFlags |= 1<<0; }
  void ICACHE_RAM_ATTR INTERRUPT_1() { InterruptsFlags |= 1<<1; }
  void ICACHE_RAM_ATTR INTERRUPT_2() { InterruptsFlags |= 1<<2; }
  void ICACHE_RAM_ATTR INTERRUPT_3() { InterruptsFlags |= 1<<3; }
  void ICACHE_RAM_ATTR INTERRUPT_4() { InterruptsFlags |= 1<<4; }
  void ICACHE_RAM_ATTR INTERRUPT_5() { InterruptsFlags |= 1<<5; }
  void ICACHE_RAM_ATTR INTERRUPT_6() { InterruptsFlags |= 1<<6; }
  void ICACHE_RAM_ATTR INTERRUPT_7() { InterruptsFlags |= 1<<7; }
  void ICACHE_RAM_ATTR INTERRUPT_8() { InterruptsFlags |= 1<<8; }
  void ICACHE_RAM_ATTR INTERRUPT_9() { InterruptsFlags |= 1<<9; }
#ifdef ESP32
  void ICACHE_RAM_ATTR INTERRUPT_10() { InterruptsFlags |= 1<<10; }
  void ICACHE_RAM_ATTR INTERRUPT_11() { InterruptsFlags |= 1<<11; }
  void ICACHE_RAM_ATTR INTERRUPT_12() { InterruptsFlags |= 1<<12; }
  void ICACHE_RAM_ATTR INTERRUPT_13() { InterruptsFlags |= 1<<13; }
  void ICACHE_RAM_ATTR INTERRUPT_14() { InterruptsFlags |= 1<<14; }
  void ICACHE_RAM_ATTR INTERRUPT_15() { InterruptsFlags |= 1<<15; }
  void ICACHE_RAM_ATTR INTERRUPT_16() { InterruptsFlags |= 1<<16; }
  void ICACHE_RAM_ATTR INTERRUPT_17() { InterruptsFlags |= 1<<17; }
  void ICACHE_RAM_ATTR INTERRUPT_18() { InterruptsFlags |= 1<<18; }
  void ICACHE_RAM_ATTR INTERRUPT_19() { InterruptsFlags |= 1<<19; }
  void ICACHE_RAM_ATTR INTERRUPT_20() { InterruptsFlags |= 1<<20; }
  void ICACHE_RAM_ATTR INTERRUPT_21() { InterruptsFlags |= 1<<21; }
  void ICACHE_RAM_ATTR INTERRUPT_22() { InterruptsFlags |= 1<<22; }
  void ICACHE_RAM_ATTR INTERRUPT_23() { InterruptsFlags |= 1<<23; }
  void ICACHE_RAM_ATTR INTERRUPT_24() { InterruptsFlags |= 1<<24; }
  void ICACHE_RAM_ATTR INTERRUPT_25() { InterruptsFlags |= 1<<25; }
  byte interrupts_tab[]={0,1,2,3,4,5,12,13,14,15,   16,17,18,19,  21,22,23,  25,26,27,  32,33,34,35,36,  39}; // 26
#else
  byte interrupts_tab[]={0,1,2,3,4,5,12,13,14,15}; // 10
#endif


void setup() {

  #ifndef ESP32
    // суки, шо ж вы делаете в новом фреймворке бляди такие? Особенно для ESP8266
  #endif

  if(LOGISET & (1 << LOG_MAIN)) Serial.begin(115200); // Serial.begin(921600);
  #ifdef ESP32
    delay(800); // ESP32 нужно подождать
  #endif

  LOGI(LOG_MAIN, (REPER("\n\n\n\n\n\t\t\t### Restart {chip}-{FlashChipId} ###")));

    // Serial.println("\n🔍 Определение модели ESP32...");
    // // Получаем чип ID
    // Serial.printf("Chip Model: %s\n", esp_get_idf_version());
    // Serial.printf("Chip Revision: %d\n", ESP.getChipRevision());
    // Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
    // Serial.printf("Flash Chip Size: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));
    // Serial.printf("PSRAM: %s\n", psramFound() ? "Yes" : "No");
    // // Определяем модель чипа
    // esp_chip_info_t chip_info;
    // esp_chip_info(&chip_info);
    // if(chip_info.model == CHIP_ESP32) Serial.println("✅ Это ESP32");
    // else if(chip_info.model == CHIP_ESP32S2) Serial.println("✅ Это ESP32-S2");
    // else if(chip_info.model == CHIP_ESP32S3) Serial.println("✅ Это ESP32-S3");
    // else if(chip_info.model == CHIP_ESP32C3) Serial.println("✅ Это ESP32-C3 (RISC-V)");
    // else Serial.println("❌ Неизвестная модель ESP32");

  #ifdef LED_BUILTIN
    LOGI(LOG_MAIN, "LED_BUILTIN = "+String(LED_BUILTIN));
  #endif

  // флеш-память #define FORMAT_SPIFFS_IF_FAILED true
  // Serial.println("\nSPIFFS Format..."); SPIFFS.format(); Serial.println("\nSPIFFS Formatted!"); while(1) { yield(); }
  #ifdef ESP32
    if(!SPIFFS.begin(true)) Serial.println("ESP32 SPIFFS Mount Failed");
    else {
      Serial.println("\nSPIFFS totalBytes="+String(SPIFFS.totalBytes()) );
      Serial.println("SPIFFS usedBytes="+String(SPIFFS.usedBytes()) );
    }
  #else
    if(!SPIFFS.begin()) {
      Serial.println("SPIFFS Mount Failed (ESP8266), formatting...");
      if(SPIFFS.format() && SPIFFS.begin()) Serial.println("SPIFFS formatted and mounted");
      else Serial.println("SPIFFS still failed after format");
    }
  #endif

  #ifdef LED_BUILTIN
  #ifdef RESET_FORCE_PIN
      pinMode(RESET_FORCE_PIN,INPUT);
      String str = "\nRESET_FORCE_PIN ("+String(RESET_FORCE_PIN)+") = ";
      if(RESET_FORCE_PIN_SET == digitalRead(RESET_FORCE_PIN)) {
        LOGI(LOG_MAIN, str+"1, press it to 0 in 3 sec while LED blinked for FULL RESET device");
        pinMode(LED_BUILTIN,OUTPUT); for(uint16_t i=0;i<30;i++) { digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN)); delay(100); }
        if(RESET_FORCE_PIN_SET != digitalRead(RESET_FORCE_PIN)) {
          // CFSET("REPARE","1");
          // Serial.println(F("Repare Config"));
          // MOTO(DefaultConfig);
          // delay(5000);
          Serial.println("Format SPIFFS...");
          SPIFFS.format();
              digitalWrite(LED_BUILTIN,0);
          Serial.println("SPIFFS formated! Restart...");
          delay(2000);
              digitalWrite(LED_BUILTIN,0);
          ESP.restart();
        } else {
          Serial.println(str+"1 still");
        }
      } else LOGI(LOG_MAIN,str+"0");
  #endif
  #endif

  // =================================================================

  #ifdef USE_WEB
    WEB_init();
  #endif

  if(CF("TMP")=="") {
    if(is_file("/config.txt")) MOTO("/config.txt"); // запустить основную инициализацию
    else MOTO(DefaultConfig); // запустить основную инициализацию
  }

  if(CF("password") != "") CFSET("passhash", MD5(REPER(F("{password}/{chip}/{FlashChipId}"))));

  // MOTO("upgrade");
  start_timer(); // запуск таймера

} // конец void setup


//#define MStart MS.x=ESP.getCycleCount();
//#define MSend(xxx) MS.xxx+=(uint64_t)(ESP.getCycleCount()-MS.x);
//#define MSprint(name,xxx) Serial.print(name); Serial.print((float)(100*MS.xxx)/(float)total); Serial.print("% "); Serial.println(MS.xxx);

// uint32_t kloop=0;

// unsigned long ms = 0;

void loop() {

  // if( (millis() - ms) > 1000) {
  //   ms = millis();
  //   Serial.println("loop: "+String(ms));
  // }
  
  // Serial.println("\n\n==== STAGE: "+String(errst++)+" ===="); delay(2);
  // if( (unsigned long)(millis() - MS.millis) > 2000) { }

  #include "MODULES_loop.cpp"

  if(InterruptsFlags) {
      // Serial.println("============> Interrupts: "+String(InterruptsFlags));
      int i=sizeof(interrupts_tab)-1;
      for(uint32_t msk = 1<<i ; i >= 0; i-- ) { // прерывания по флагам
        // Serial.println(" ---> i = "+String(i)+" msk = "+String(msk));
        if(InterruptsFlags & msk) {
            // Serial.println("============> Interrupts RUN: INT_"+String(interrupts_tab[i])+"\n[\n"+ CF("INT_"+String(interrupts_tab[i])) +"\n]\n" );
            MOTO(CF("INT_"+String(interrupts_tab[i])), 0);
            InterruptsFlags &= ~msk;
            if(InterruptsFlags) break;
        }
        msk = msk >> 1;
     }
  }

 String s=CF("MAINLOOP");
 if(s!="") MOTO(CF("MAINLOOP"), 0);

  if(Breath.TT) breath();

  if(SheduleInit) shedule();

  #ifdef USE_UDP
    if(WIFIudp_flag>0) WIFIudp_result(); // Если ждем ответа на UDP-запрос
  #endif

  for (uint8_t i = 0; i < NLOOP; i++) {
      if (Loopes[i].value == 0) continue; // цикл отсутствует
      if (Loopes[i].count >= Loopes[i].value) { // пришло время по секундам
        Loopes[i].count = 0; // обнулить время по секундам
        if (MOTO(Loopes[i].file, Loopes[i].yild) == "stop") Loopes[i].value = 0; // завершить выполнение и не повторять более
        if (Loopes[i].one != 0) {
          if (--Loopes[i].one == 0) Loopes[i].value = 0;  // остановить
        }
        break; // только одну за раз!
      }
  }

}

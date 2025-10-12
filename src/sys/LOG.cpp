#include <Arduino.h>
#include "LOG.h"

/*
#define LOGNAMES_N 9
// const String LOGNAMES [LOGNAMES_N] = { "MAIN" , "ERROR", "WEB", "WGET", "UPGRADE", "MP3", "IECHO", "PROG", "CONN"};
#define LOG_MAIN 0 // системные логи типа старта и инициаизация serial вообще, если 0, то нет логов
#define LOG_ERROR 1 // ошибки системы
#define LOG_WEB 2 // логи веб-запросов
#define LOG_WGET 3 // логи запросов данных на внешний сервер
#define LOG_UPGRADE 4 // лога апгрейда
#define LOG_MP3 5 // логи MP3 отдельно
#define LOG_IECHO 6 // логи iecho
#define LOG_PROG 7 // логи выполнения MOTO
#define LOG_CONN 8 // логи сервисов типа MQTT и MODBUS
*/
uint16_t LOGISET = LOGISET_DEFAULT; // по умолчанию логи 

String LOG_I_NAME(byte i) {
  return LOGNAMES[i];
}

byte LOG_NAME_I(String s) {
  for (byte i = 0; i < LOGNAMES_N; i++) {
    if (s == LOGNAMES[i]) return i;
  } return 0;
}

void LOGI(byte i, String s, String music) {
  LOGI(i, s);
  play(music);
}

void LOGI(byte i, String s) { LOGI(i,s,(s.length()>2 ? 0: 1) ); }
void LOGI(byte i, String s, byte noN) {
  if( LOGISET & (1 << i) ) {
    // yield();
    s.replace("\\n", "\n[" + LOGNAMES[i] + "] --> ");
    //   if(s.length() > 3)
    if(!noN) Serial.print("\n");
    Serial.print(s);
  }
}

bool LOGLI(byte i) {
  return LOGISET & (1 << i);
}

// void LOG(String s) { yield(); s.replace("\n","\\n"); Serial.println("log: --> "+s); }
String ERR(String s) {
  s.replace("\n", "\\n");
  LOGI(LOG_PROG, "ERROR: ============> " + s);
  return "";
}
String ERR(String s, String o) {
  return ERR(s + " [" + o + "]");
}

String LOG(String s) { return LOG(s,0); }
String LOG(String s,byte noN) {
  s.replace("\n", "\\n");
  LOGI(LOG_PROG, (s.length()>3?" ===> ":"") + s,noN);
  return "";
}

// LOGGG

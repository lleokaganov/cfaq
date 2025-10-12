#include <Arduino.h>
#include "udp.h"

// основная процедура, если flag<0, то задо запустить *.begin(); Если 0 - свободен, если 1 - своя процедура, если 123 - time

WiFiUDP WIFIudp; // udp для всяких нужд
int8_t WIFIudp_flag=-1; // требует запуска .begin при первом обращении
uint32_t WIFIudp_timestamp=0; // время, чтобы следить за приходящими пакетами

// вообще непонятно откуда осталось:
WiFiUDP MEudp; // udp для безответной отправки, ничего не требует лишнего
byte ME_status=0; // это тоже про UDP

// ================== ping UDP ==========

void WIFIudp_result(void) {
    int n=WIFIudp.parsePacket();
    if(!n) {
      if(ESP.getCycleCount()-WIFIudp_timestamp > 80300859) {
        LOG("UDP no answer for ["+String(WIFIudp_flag)+"]");
        WIFIudp_flag=0;
      }
      return;
    }

    #define NTP_PACKET_SIZE 48
    #define UDP_PACKET_MAX 512
    if(WIFIudp_flag==123 && n>NTP_PACKET_SIZE) n=NTP_PACKET_SIZE; // а то мало ли, сколько пришлет NTP-сервис
    else if(n>UDP_PACKET_MAX) n=UDP_PACKET_MAX; // и вообще ограничим приемку 512 байтами

    byte *Bu; free(Bu); Bu=(byte*)malloc(n+1);
    n=WIFIudp.read(Bu,n); *(Bu+n)=0;

    if(WIFIudp_flag==123) { // Set time
      unsigned long highWord = word(Bu[40], Bu[41]);
      unsigned long lowWord = word(Bu[42], Bu[43]);
      unsigned long T = ( highWord << 16 | lowWord ) - 2208988800UL;
      LOG("NTP unixtime="+String(T));
      if(T<1659389434) LOG("SET Time Error ("+String(T)+")");
      else {
        LOG("SET Time "+Time_hhmmss(T)+" Difference="+String( (int)(T - UnixTime) ) );
        UnixTime=T;
      }
    } else if(WIFIudp_flag==124) { // MOTO
        String s = String((char*)Bu);
        LOG("UDP recieved answer: ["+s+"]");
        MOTO( s );
    } else if(WIFIudp_flag==125) { // UPGRADE
        String s = String((char*)Bu);
        LOG("UDP recieved answer: ["+s+"]");
        UpgradeALL_res(s);
    } else { // получить результат
        CFSET("UDP_result", String((char*)Bu));
        LOG("UDP get answer");
        MOTO(CF("UDP_func"));
    }
    free(Bu);
    WIFIudp_flag=0;
}

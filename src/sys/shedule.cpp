#include <Arduino.h>
#include "main.h"

String SheduleS="";
// byte SheduleInit=0;


void oshedule(String key) {
    int x=SheduleS.indexOf("\n"+key+" ");
    if(x >= 0) loop_now( PARG(SheduleS.substring(x+key.length()+2),0,"\n") );
}

void shedule(void) {
  
  static uint32_t OU=0;
  if(UnixTime <= OU) return;
  uint32_t UT=UnixTime; // шоб без сюрпризов
  if(UT < OU+10) { // максимум до 10 секунд зависаний хотим обрабатывать
    String hh = Time_hh(UT);
    String mm = Time_mm(UT);
    String ss = Time_ss(UT);
    String dn = Time_dn(UT);
    String ddn = Time_ddn(UT);
    for(int i=0;i<10;i++) { OU++;
      oshedule(dn+"/"+hh+":"+mm+":"+ss);
      oshedule(ddn+"/"+hh+":"+mm+":"+ss);
      oshedule(hh+":"+mm+":"+ss);
      oshedule("*:"+mm+":"+ss);
      oshedule("*:*:"+ss);
      oshedule("*:*:*");
      if(ss=="00") {
               oshedule(hh+":"+mm);
               oshedule(hh+":"+mm+":*");
               if(mm=="00") {
                      oshedule(hh);
                      oshedule(hh+":*");
                      if(hh=="00") oshedule(ddn);
               }
      }
      if(OU = UT) break;
     }
  }
  OU = UT;
}

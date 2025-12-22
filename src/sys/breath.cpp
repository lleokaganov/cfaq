#include <Arduino.h>
// #include "breath.h"
// #include "wifi.h"
#include "main.h"

BreathStruct Breath = {
  .TT = 180000,
  .TTT = 60000000,
  .pinok = 2,
  .Z = 600,
  .min = 500,
  .max = 1200,
  .K = 1,
  .CY = 0
};

void breath(void) {
  //#define TT 180000
  //#define TTT 60000000
  //#define pinok 2
  // uint32_t TT = CF0("TT");
  // uint32_t TTT = CF0("TTT");
  //static uint16_t Z=600;
  //static int16_t K=1;
  //static uint32_t CY=0;

  uint32_t C=ESP.getCycleCount();
  if(C-Breath.CY < Breath.TT) return;

  if(ISNET) {
    if(Breath.Z < Breath.min || Breath.Z > Breath.max) Breath.K*=-1;
    Breath.Z+=Breath.K;
    analogWrite(Breath.pinok,Breath.Z);
    // LOG(String(Breath.pinok)+"="+String(Breath.Z));
  } else {
    if(C-Breath.CY < Breath.TTT) return;
    digitalWrite(Breath.pinok,!digitalRead(Breath.pinok));
  }
  Breath.CY=C;
}

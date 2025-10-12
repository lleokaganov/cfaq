// --- wiegand ---
if(cmd=="WIEGAND.start") {
  byte pin0=PARG0(s,1),pin1=PARG0(s,2),mod=PARG0(s,3);
  if(mod) { if(mod!=26 && mod!=34) return ERR(cmd); wiegand_mode=mod; }
  wiegand=0;
  wiegand_n=0;
  wiegand_pins=pin0+(pin1<<4); // храним в байте для экономии
  pinMode(pin0,INPUT_PULLUP);
  pinMode(pin1,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin0), ISR_WIEGAND_0, FALLING);
  attachInterrupt(digitalPinToInterrupt(pin1), ISR_WIEGAND_1, FALLING);
  return LOG(cmd+" pin0:"+String(pin0)+" pin1:"+String(pin1)+" mode:"+mod);
}

if(cmd=="WIEGAND.end") {
  byte pin0 = wiegand_pins & 0x0F , pin1 = (wiegand_pins >> 4) & 0x0F;
  detachInterrupt(digitalPinToInterrupt(pin0));
  detachInterrupt(digitalPinToInterrupt(pin1));
  return LOG(cmd+" "+String(pin0)+" "+String(pin1));
}
/*
Red +12
Black GND
Green D0/TX/4R+
White D1/RX/4R-

Blue LED/CS
Yellow BEEP
Grey(Gray) WG26/34 ( ? Beepr)

Purple Door Bell + ( ? GND Wiegand 34)
Purple Door Bell -


56|5ADC379D|7A
1000|5ADC379D
1524381597 23260,14237
W26: 0014432157 220,14237

160066CAB50F
0000|0066CAB5|
0006736565 102,51893
*/
// --- wiegand ---


// === LAN ===
// MOSI    = GPIO13
// MISO    = GPIO12
// SCK     = GPIO14
// CSPIN ... SET (GPIO15?)
// 3v
// GND
// RST, INT и NC - не используются


//----------- wiegand -----------
byte wiegand_mode = 34;  // max number of bits (26 or 34)
byte wiegand_pins;

volatile uint64_t wiegand;
volatile uint8_t wiegand_n;
// interrupt that happens when INTO goes low (0 bit)
void ICACHE_RAM_ATTR ISR_WIEGAND_0() {
  wiegand = (wiegand << 1);
  // uint32_t w=wiegand;
  // Serial.println("W1: "+String(w,BIN));
  // if(wiegand_n < wiegand_mode) { wiegand++; = (wiegand << 1);
  wiegand_n++;
}
// interrupt that happens when INT1 goes low (1 bit)
void ICACHE_RAM_ATTR ISR_WIEGAND_1() {
  wiegand = (wiegand << 1) + 1;
  // uint32_t w=wiegand;
  // Serial.println("W0: "+String(w,BIN));
  // if(wiegand_n < wiegand_mode) { wiegand++;  = (wiegand << 1)+1;
  wiegand_n++;
}
//----------- wiegand -----------

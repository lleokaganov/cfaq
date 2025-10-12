  if(cmd=="SPI.begin") { // SPI.begin sck miso mosi ss
    int8_t sck = PARG0(s,1," ",-1); if(sck<0) { SPI.begin(); return ""; }
    int8_t miso = PARG0(s,2," ",-1);
    int8_t mosi = PARG0(s,3," ",-1);
    int8_t ss = PARG0(s,4," ",-1);
    SPI.begin(sck, miso, mosi, ss);
    return "";
 }

  if(cmd=="SPI.end") { SPI.end(); return ""; }

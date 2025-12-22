  if(CMD_EQ("SPI.begin")) { // SPI.begin sck miso mosi ss
    int8_t sck = PARG0(s,1," ",-1); if(sck<0) { SPI.begin(); return EMPTY_STRING; }
    int8_t miso = PARG0(s,2," ",-1);
    int8_t mosi = PARG0(s,3," ",-1);
    int8_t ss = PARG0(s,4," ",-1);
    SPI.begin(sck, miso, mosi, ss);
    return EMPTY_STRING;
 }

  if(CMD_EQ("SPI.end")) { SPI.end(); return EMPTY_STRING; }

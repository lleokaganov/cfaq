
#if defined(ESP32)
  #define I2C_DEFAULT_SDA 21
  #define I2C_DEFAULT_SCL 22
#else
  #define I2C_DEFAULT_SDA 4
  #define I2C_DEFAULT_SCL 5
#endif

/// I2C.begin 21 22 [frequency=100000] [timeout_ms=50]
if(CMD_EQ("I2C.begin")) { // I2C.begin SDA SCL
    I2C_pin_SDA = PARG0(s,1," ",I2C_DEFAULT_SDA);
    I2C_pin_SCL = PARG0(s,2," ",I2C_DEFAULT_SCL);
    uint32_t frequency = PARG0(s,3," ",100000);
    I2C_timeout_ms = PARG0(s,4," ",I2C_timeout_ms);
    Wire.begin(I2C_pin_SDA, I2C_pin_SCL);
    Wire.setClock(frequency);
    Wire.setTimeout(I2C_timeout_ms);
    return EMPTY_STRING;
}

/// I2C.setClock 100000
if(CMD_EQ("I2C.setClock")) { // I2C.setClock FREQUENCY
    uint32_t frequency = PARG0(s,1);
    Wire.setClock(frequency);
    return EMPTY_STRING;
}

/// I2C.end
if(CMD_EQ("I2C.end")) {
    I2C_pin_SDA = 0xFF;
    I2C_pin_SCL = 0xFF;
    Wire.end();
    return EMPTY_STRING;
}

/// I2C.write DEV REG VAL1 [VAL2 ...]
if(CMD_EQ("I2C.write")) {
    uint8_t res = I2C_write( PARG0(s,1), PARG0(s,2), ARG_OTHER(s,2) );
    return EMPTY_STRING;
}

// I2C.beginTransmission 0x78
if(CMD_EQ("I2C.beginTransmission")) {
  Wire.beginTransmission( PARG0(s,1));
  return EMPTY_STRING;
}

if(CMD_EQ("I2C.timeout")) {
  I2C_timeout_ms = PARG0(s,1," ",I2C_timeout_ms);
  Wire.setTimeout(I2C_timeout_ms);
  return EMPTY_STRING;
}

// if(CMD_EQ("I2C.test")) {
//   uint8_t dev = PARG0(s,1," ",0x26); // попробовать 0x24, 0x25, 0x26, 0x48
//   uint8_t n = PARG0(s,2," ",3); // попробовать 3

//   Serial.print("I2C.test: "+HH0X(dev)+" N="+String(n)+" ");

//   Wire.requestFrom((uint8_t)dev, (uint8_t)n, (uint8_t)1);   // читаем 3 байта (возможно, 2)
//   if (!Wire.available() ) {
//     Serial.println(" not available");
//     return EMPTY_STRING;
//   }
//   uint32_t value = 0;
//   int stop = I2C_BUF_MAX_SIZE;
//   while (--stop && Wire.available()) { 
//     uint8_t b = Wire.read();
//     Serial.print(" "+HH0X(b));
//     value = (value << 8) | b;
//   }
//   Serial.print(" Raw ADC: "+String(value)+" ");
//   return EMPTY_STRING;
// }

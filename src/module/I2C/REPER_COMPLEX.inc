if(CMD == F("I2C")) {
    uint8_t nBytes = PARG0(l,2);
    if(nBytes > I2C_BUF_MAX_SIZE) return "ERROR";
    uint8_t I2C_buf[nBytes];
    if( ! I2C_read(PARG0(l,0), PARG0(l,1), nBytes, I2C_buf) ) return "error";
    String o="";
    for(int i=0; i<nBytes; i++) o += String(i==0?"":" ") + HH0X(I2C_buf[i]);
    return o;
}

if(CMD == F("I2C.8")) {
    uint8_t I2C_buf[1];
    Serial.printf("!!! I2C.8 dev: 0x%02X reg: 0x%02X nbuf=%d\n", PARG0(l,0), PARG0(l,1), 1); // DEBUG
    if( ! I2C_read(PARG0(l,0), PARG0(l,1), 1, I2C_buf) ) return "error";
    return String(I2C_buf[0]);
}

if(CMD == F("I2C.16")) {
    uint8_t I2C_buf[2];
    if( ! I2C_read(PARG0(l,0), PARG0(l,1), 2, I2C_buf) ) return "error";
    return String((I2C_buf[0] << 8) | I2C_buf[1]);
}

/// echo {I2C.24:DEV REG}
if(CMD == F("I2C.24")) {
    uint8_t I2C_buf[3];
    if( ! I2C_read(PARG0(l,0), PARG0(l,1), 3, I2C_buf) ) return "error";
    return String(( (uint32_t)I2C_buf[0] << 16 ) | ( (uint32_t)I2C_buf[1] << 8 ) | (uint32_t)I2C_buf[2]);
}

if(CMD == F("I2C.32")) {
    uint8_t I2C_buf[4];
    if( ! I2C_read(PARG0(l,0), PARG0(l,1), 4, I2C_buf) ) return "error";
    return String(( (uint32_t)I2C_buf[0] << 24 ) | ( (uint32_t)I2C_buf[1] << 16 ) | ( (uint32_t)I2C_buf[2] << 8 ) | (uint32_t)I2C_buf[3]);
}

/// {I2C.WRITE:DEV:REG:0x01 0x01 0x02...}
if(CMD == F("I2C.WRITE")) {
    uint8_t res = I2C_write( PARG0(l,0), PARG0(l,1), ARG_OTHER(l,1) );
    return String(res);
}

// /// echo {I2C.PING:DEV [LEN] [TIMEOUT_MS]}
// if(CMD == F("I2C.PING")) {
//     uint8_t dev = PARG0(l,0);
//     uint32_t timeout_ms = PARG0(l,1," ",0);
//     bool res = i2c_ping_fast(dev, timeout_ms);
//     return String(res ? "1" : "0");
// }
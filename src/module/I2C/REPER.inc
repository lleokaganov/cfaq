/// I2C.SCAN
if(l=="I2C.SCAN") {
  String o="";
  for (byte i = 1; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) o += (o==""?"":",") + HH0X(i);
  }
  return o;
}

if(l=="I2C.endTransmission") {
   uint8_t error = Wire.endTransmission();
   return String(error);
}






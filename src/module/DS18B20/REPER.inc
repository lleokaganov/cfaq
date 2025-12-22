if(l == F("DS18B20.SCAN")) {
  if(DS18B20 == nullptr) return "ERROR";
  DeviceAddress addr;
  String o="";
  for (uint8_t i = 0; i < DS18B20->getDeviceCount(); i++) {
    DS18B20->getAddress(addr, i);
    o+=(i?",":"")+String(i)+":"+DS18B20_addr2string(addr);
  }
  return o;
}

if(l == F("DS18B20.ALL")) {
  if(DS18B20 == nullptr) return "ERROR";
  DS18B20->requestTemperatures();
  DeviceAddress addr;
  String o="";
  for (uint8_t i = 0; i < DS18B20->getDeviceCount(); i++) {
    DS18B20->getAddress(addr, i);
    float t = DS18B20->getTempC(addr);
    o+=(i?",":"")+DS18B20_addr2string(addr)+"="+(t == DEVICE_DISCONNECTED_C ? "error" : String(t, 2) );
  }
  return o;
}


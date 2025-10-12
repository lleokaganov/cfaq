
  if(l == F("DS18B20")) {
    if(DS18B20 == nullptr) return "error";
    DS18B20.requestTemperatures();
    return String(DS18B20.getTempCByIndex(0));
  }





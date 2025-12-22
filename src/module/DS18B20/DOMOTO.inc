
// DS18B20.begin

if(CMD_EQ("DS18B20.begin")) { // запустить после ONEWIRE pin
  if(DS18B20 != nullptr) delete DS18B20;
  DS18B20 = new DallasTemperature(oneWire);
  DS18B20->begin();
  return EMPTY_STRING;
}

if(CMD_EQ("DS18B20.request")) { // запустить после ONEWIRE pin
  if(DS18B20 != nullptr) DS18B20->requestTemperatures();
  return EMPTY_STRING;
}

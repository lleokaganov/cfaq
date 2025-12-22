if(CMD == F("DS18B20")) {
    if(DS18B20 != nullptr) DS18B20->requestTemperatures();
    CMD = F("DS18B20.GET");
}

if(CMD == F("DS18B20.GET")) {
    if(DS18B20 == nullptr) return "ERROR";
    String id = PARG(l,0," ","0");
    float t;
    if (id.length() != 16) t = DS18B20->getTempCByIndex(id.toInt());
    else {
        DeviceAddress addr;
        DS18B20_string2addr(id, addr);
        t = DS18B20->getTempC(addr);
    }
    if (t == DEVICE_DISCONNECTED_C) return "error";
    return String(t, 2);
}
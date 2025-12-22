// DS18B20 Temperature Sensor

#include "module/DS18B20/h.hpp"

DallasTemperature* DS18B20 = nullptr;

String DS18B20_addr2string(DeviceAddress addr) {
    String o="";
    for (uint8_t i=0; i<8; i++) {
        if(addr[i]<16) o+="0";
        o+=HH(addr[i]);
    }
    o.toUpperCase();
    return o;
}

void DS18B20_string2addr(String s, DeviceAddress addr) {
    for (uint8_t i = 0; i < 8; i++) {
        String c = s.substring(i*2, i*2+2);
        addr[i] = (uint8_t) strtol(c.c_str(), nullptr, 16);
    }
}
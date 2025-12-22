#ifndef DS18B20_LIB_H
#define DS18B20_LIB_H

#include <DallasTemperature.h>
extern DallasTemperature* DS18B20;

String DS18B20_addr2string(DeviceAddress addr);
void DS18B20_string2addr(String s, DeviceAddress addr);

#endif
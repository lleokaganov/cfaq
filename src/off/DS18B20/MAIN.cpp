//---------------- Dallas
/* DS18B20 Temperature Sensor */

#include <OneWire.h>
#include <DallasTemperature.h>

// Указатели на объекты OneWire и DallasTemperature
OneWire* oneWire = nullptr;
DallasTemperature* DS18B20 = nullptr;

// #define WIREPIN 4
// OneWire oneWire(WIREPIN);
// DallasTemperature DS18B20(&oneWire);

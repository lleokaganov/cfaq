/*
    ModbusIP_ESP8266.h - Header for Modbus IP ESP8266 Library
    Copyright (C) 2015 Andr� Sarmento Barbosa
*/

// #define LOG_CONN 8 // заебало
// void LOGI(byte i,String s,String play);
// void LOGI(byte i,String s);



#include "Modbus.h"
#include <ESP8266WiFi.h>

#ifndef MODBUSIP_ESP8266_H
#define MODBUSIP_ESP8266_H

#define MODBUSIP_PORT 	  502
#define MODBUSIP_MAXFRAME 200
#define MODBUSIP_TIMEOUT   10

class ModbusIP : public Modbus {
    private:
        byte _MBAP[7];
        
        int available();
        int connected();
        char read();
        // void write(uint8_t c);
        void write(uint8_t *buf, size_t size);
        void stop();
        void flush();
        
    public:
        ModbusIP();
        // void config(const char* ssid, const char* password);
        void begin();
        void task();
        char last[12]={0xAA};
};

#endif //MODBUSIP_ESP8266_H

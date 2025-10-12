/*
    ModbusIP_ESP8266.cpp - Source for Modbus IP ESP8266 Library
    Copyright (C) 2015 Andr� Sarmento Barbosa
*/
#include "ModbusIP_ESP8266.h"
#include <Ethernet2.h>

void dd(byte x) { char s[3]; sprintf(s,"%02X",x); Serial.print(" "+String(s)); }

WiFiServer MODBUS_server(MODBUSIP_PORT);
WiFiClient mcl;
EthernetServer MODBUS_LANserver(MODBUSIP_PORT);
EthernetClient LAN_mcl;
boolean ModSource=1;

int ModbusIP::available() { return (ModSource ? mcl.available() : LAN_mcl.available() ); }
int ModbusIP::connected() { return (ModSource ? mcl.connected() : LAN_mcl.connected() ); }
char ModbusIP::read() { return (ModSource ? mcl.read() : LAN_mcl.read() ); }
void ModbusIP::write(uint8_t *buf, size_t size) { if(ModSource) mcl.write(buf,size); else LAN_mcl.write(buf,size); }
void ModbusIP::stop() { if(ModSource) mcl.stop(); else LAN_mcl.stop(); }
void ModbusIP::flush() { if(ModSource) mcl.flush(); else LAN_mcl.flush(); }

ModbusIP::ModbusIP() { }

void ModbusIP::begin() { MODBUS_server.begin(); }

void ModbusIP::task() {
  
	if(!mcl) mcl = MODBUS_server.available();
  if(mcl && mcl.connected()) { ModSource=1; }
  else { // а теперь попробуем тогда LAN
      LAN_mcl = MODBUS_LANserver.available();
      if(!LAN_mcl || !LAN_mcl.connected()) return;
      ModSource=0;
  }

  uint16_t raw_len = 0;

  for (uint16_t x = 0; x < 300; x++) { // Time to have data available
			if ( this->available()) {
					while (this->available() > raw_len) {  //Computes data length
						raw_len = this->available();
						delay(1);
					}
					break;
				}
				delay(10);				
	}


  // LOGI(LOG_CONN,"MODBUS "+String(raw_len)+" [");

  if (raw_len <= 7) { this->stop(); return; }

  byte last_i=0;

	for (byte i=0; i<7; i++)	{
		  _MBAP[i] = this->read(); //Get MBAP
       dd(_MBAP[i]);
       last[last_i++]=_MBAP[i];
	}

	_len = _MBAP[4] << 8 | _MBAP[5];
	_len--; // Do not count with last byte from MBAP
	if (_MBAP[2] !=0 || _MBAP[3] !=0) {
        // LOGI(LOG_CONN," ] Not a MODBUSIP packet");
      this->stop();
	    return;   // Not a MODBUSIP packet
	}
 
	if (_len > MODBUSIP_MAXFRAME) {
       // LOGI(LOG_CONN," ] Length "+String(_len)+" over");
	    return;      //Length is over MODBUSIP_MAXFRAME
	}
	
	_frame = (byte*) malloc(_len);
			
	raw_len = raw_len - 7;

  // LOGI(LOG_CONN," / ");
	for (uint16_t i=0; i< raw_len; i++) {
		  _frame[i] = this->read(); //Get Modbus PDU
       if(last_i<12) last[last_i++]=_MBAP[i];
       dd(_frame[i]);
	}
  // LOGI(LOG_CONN," ] ");

  while(last_i<12) last[last_i++]=0xAA;
      
	this->receivePDU(_frame);
	// this->flush();

	if (_reply != MB_REPLY_OFF) {
	  //MBAP
		_MBAP[4] = (_len+1) >> 8;     //_len+1 for last byte from MBAP
		_MBAP[5] = (_len+1) & 0x00FF;

		size_t send_len = (unsigned int)_len + 7;
		uint8_t sbuf[send_len];
				
		for (byte i=0; i<7; i++)	    sbuf[i] = _MBAP[i];
		for (uint16_t i=0; i<_len; i++)	sbuf[i+7] = _frame[i];

    // LOGI(LOG_CONN," -> { "); for (int i=0; i<send_len; i++) dd(sbuf[i]); // LOGI(LOG_CONN," }");

		this->write(sbuf, send_len);
    this->flush();
	}

  // LOGI(LOG_CONN,"\n");
	// this->stop();
	free(_frame);
	_len = 0;
}

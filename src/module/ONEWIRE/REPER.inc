if(l == F("ONEWIRE.is")) {
	return (oneWire != nullptr && oneWire->reset() ? "1" : "0");
}

if(l == F("ONEWIRE.SCAN")) {
	if(oneWire == nullptr || !oneWire->reset()) return "error";
    byte addr[8];
    int count = 0;
	String o="";
    while (oneWire->search(addr)) {
		o+=(count?",":"")+String(count++)+":";
		for(byte i=0; i<8; i++) o+=HH(addr[i]);
		// Проверим CRC
        if(OneWire::crc8(addr,7) != addr[7]) o+="_ERRCRC";
		else {		
	    	// Определим тип устройства по первому байту
	    	switch (addr[0]) {
    			case 0x28: o+="_DS18B20"; break;
    			case 0x10: o+="_DS18S20"; break;
    			case 0x22: o+="_DS1822";  break;
    		}
		}
    }
    oneWire->reset_search(); // сброс поиска
    o.toUpperCase();
	return o;
}

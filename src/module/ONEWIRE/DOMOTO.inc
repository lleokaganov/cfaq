/// ONEWIRE.begin [pin]
if(CMD_EQ("ONEWIRE.begin")) {
    int pin = PARG0(s,1," ",0);
    if(oneWire != nullptr) delete oneWire;
    oneWire = new OneWire(pin);
    return EMPTY_STRING;
}

/// ONEWIRE.end
if(CMD_EQ("ONEWIRE.end")) {
    if(oneWire == nullptr) return EMPTY_STRING;
    delete oneWire;
    oneWire = nullptr;
    int pin = PARG0(s,1," ",-1);
    if(pin != -1) pinMode(pin, INPUT);
    return EMPTY_STRING;
}

if(CMD_EQ("ONEWIRE.reset")) {
    if(oneWire == nullptr) return EMPTY_STRING;
	oneWire->reset();
    return EMPTY_STRING;
}
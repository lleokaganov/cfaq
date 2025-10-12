
/// PN532.init SDA SCL [WIRE]
/// PN532.init 42 41 0
/// = {PN532.ver}
/// = {PN532.card}

// PN532.end
// PN532.init 42 41 1
// echo VER={PN532.ver}
// repeat 100 {
//    set card = {PN532.card}
//    if.!empty {card} echo {card}
// }
// echo VER={PN532.ver}

if(cmd=="PN532.init") {
    if(PN532_nfc != nullptr) { delete PN532_nfc; PN532_nfc = nullptr; delay(200); }
    #ifdef ESP32
	TwoWire* chosenWire = PARG0(s,3," ",0) ? &wire1 : &wire0 ;
        chosenWire->begin( PARG0(s,1," ",42), PARG0(s,2," ",41) ); // SDA, SCL - любые пины
        PN532_nfc = new Adafruit_PN532(-1, -1, chosenWire);
    #else
	Wire.begin( PARG0(s,1," ",4), PARG0(s,2," ",5) ); // SDA=4, SCL=5 рекомендованы в ESP8266, можно менять не на все
        PN532_nfc = new Adafruit_PN532(-1, -1, &Wire);
    #endif
    PN532_nfc->begin();
    if(!PN532_nfc->getFirmwareVersion()) { ERR("PN532 not found"); return ""; }
    PN532_nfc->SAMConfig();
    PN532_timeout = PARG0(s,4," ",50);
    return "";
}

if(cmd == "PN532.end") {
    if(PN532_nfc == nullptr) return "";
    delete PN532_nfc;
    PN532_nfc = nullptr;
    return "";
}
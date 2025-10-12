if(l == F("PN532.ver")) {
    if(PN532_nfc == nullptr) return "ERROR";
    uint32_t v = PN532_nfc->getFirmwareVersion();
    if(!v) return "";
    return String(v, HEX);
}

if(l == F("PN532.card")) {
    if(PN532_nfc == nullptr) return "ERROR";
//    PN532_nfc->SAMConfig();
    uint8_t uid[7];
    uint8_t uidLength;
    if(!(PN532_nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, PN532_timeout))) return ""; // 50ms
    String o=""; for(uint8_t i=0; i<uidLength && i<7; i++) o+=(uid[i]<0x10?"0":"")+String(uid[i],HEX);
    o.toUpperCase();
    return o;
}

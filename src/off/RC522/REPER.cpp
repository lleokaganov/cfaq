  // == MFRC522 ==

  if(l == F("RC522.ver")) {
	return String(mfrc522.PCD_ReadRegister(mfrc522.VersionReg), HEX); // 0x91 или 0x92
  }

  if(l == F("RC522.card")) {
    if( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return "";
    byte n = mfrc522.uid.size;
    if(!n) return "error";
    String o = "";
    for(byte i = 0; i < n; i++) o += String(mfrc522.uid.uidByte[i], HEX); o.toUpperCase();
    return o;
  }


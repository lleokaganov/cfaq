// ============ RC522 ==========
  if(cmd=="RC522.init") {
	if(NFC_init) {
	    SPI.end();
	    NFC_init=0;
	}
	NFC_init=1;
	SPI.begin();
	mfrc522.PCD_Init();
	mfrc522.PICC_HaltA();
	return LOG(cmd);
	// return "";
  } // rfid.halt();
//  if(cmd=="SPI.begin") { SPI.begin(); return ""; }
//  if(cmd=="SPI.end") { SPI.end(); return ""; }
// ============ RC522 ==========

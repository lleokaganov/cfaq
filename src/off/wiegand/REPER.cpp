  // == WIEGAND ==
  if (l == F("WIEGAND")) {
    if (wiegand_n < wiegand_mode) return "";
    uint64_t w = wiegand >> 1;
    byte w0, i = 0, parity0 = 0, parity1 = 0;

    for (; i < 16; i++) { parity0 += w & 1; w >>= 1; }
    for (; i < 32; i++) { parity1 += w & 1; w >>= 1; }
    if ( parity1 & 1 == wiegand & 1 || parity1 & 1 != w) return "error_parity1: " + String(parity1 & 1) + " != " + String( (wiegand & 0x100000000 ? 1 : 0) ) + "\n";

    w = ( wiegand >> 1 ) & 0xFFFFFFFF; String o = "";  for (i = 0; i < 8; i++) {
      w0 = w & 0xF;
      o = String(w0, HEX) + o;
      w >>= 4;
    } o.toUpperCase();
    // w = wiegand; o="";  for(i=0;i<34;i++) { w0 = w & 1; o=" "+String(w0,BIN)+o; w >>= 1; } oe+=o+"\n";
    wiegand = 0; wiegand_n = 0;
    return o;
  }
  // == WIEGAND ==

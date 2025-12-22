if(l == F("WS.x")) return bin2hex(x_public, 32);
if(l == F("WS.ed")) return bin2hex(ed_public, 32);
if(l == F("WS.connected")) return (ws.is_connected() ? "1" : "0");
if(l == F("WS.seed")) return crypto25519_seed();


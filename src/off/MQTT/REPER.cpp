if(l == F("MQTT")) {
    return String(MQTT && client.connected() ? "1" : "0");
}
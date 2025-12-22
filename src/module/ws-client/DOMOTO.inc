/// WS.set.my_keys 2C54.BAEB
if( cmd == "WS.set.my_keys") {
    String seeds = PARG_OTHER(s, 0);
    if(!import_my_keys( seeds )) ERR("Failed to import my key");
    return EMPTY_STRING;
}

/// WS.set.key 0-FC58-9AE6
if( cmd == "WS.set.key") {
    String key = PARG_OTHER(s, 0);
    if(!import_key( key )) ERR("Failed to import key");
    return EMPTY_STRING;
}

/// WS.begin ws://10.1.1.7:8112/ws/user/v1/<ed_public>
if (cmd == "WS.begin") {

    if (ws.is_connected()) ws.stop();

    url_struct url = url_parse(PARG_OTHER(s, 0));
    if( url.protocol != "ws" && url.protocol != "http") {
        Serial.println("===> WS: Invalid protocol: " + url.protocol);
        return EMPTY_STRING;
    }

    ws.begin(url.host, url.port, url.path);
    ws.onMessageText(ws_onMessageText_cb);
    ws.onMessageBin(ws_onMessageBin_cb);
    ws.onConnect(ws_onConnect_cb);
    ws.onDisconnect(ws_onDisconnect_cb);

    return EMPTY_STRING;
}

if (cmd == "WS.stop") {
    if (ws.is_connected()) ws.stop();
    return EMPTY_STRING;
}


/// отправить данные без ожидания ответа
/// WS.SECURE 0x00 12345 0 "{\"action\":\"my_info\"}"
if (cmd == "WS.SECURE") {
    if (!ws.is_connected()) return ERR("WS not connected");
    uint8_t cmd_s = (uint8_t)PARG0(s, 1);
    uint32_t to = (uint32_t)PARG0(s, 2);
    uint16_t message_id = (uint32_t)PARG0(s, 3);
    String body = PARG_OTHER(s, 3);
    ws.send_secret(cmd_s, body, to, message_id);
    return EMPTY_STRING;
}

/// отправить данные без ожидания ответа, синоним WS.SECURE 0x00 0 0
/// WS.SERVER {"action":"my_info"}
if (cmd == "WS.SERVER") { // отправить данные без ожидания ответа
    if (!ws.is_connected()) return ERR("WS not connected");
    String body = PARG_OTHER(s, 0);
    Serial.println("WS.SERVER: [" + body + "]");
    ws.send_secret(0x00, body, 0);
    return EMPTY_STRING;
}

/// отправить данные в базу сервера, синоним WS.SECURE 0x10 0 0
/// WS.DATA {"action":"my_info"}
if (cmd == "WS.DATA") { // отправить данные без ожидания ответа
    if (!ws.is_connected()) return ERR("WS not connected");
    String body = PARG_OTHER(s, 0);
    Serial.println("WS.DATA: @@[" + String(body) + "]@@");
    ws.send_secret(0x10, body, 0);
    return EMPTY_STRING;
}

/// отправить ответ
/// WS.DATA "{\"action\":\"my_info\"}"
if (cmd == "WS.ANSWER") { // отправить данные без ожидания ответа
    if (!ws.is_connected()) return ERR("WS not connected");
    uint16_t message_id = (uint32_t)PARG0(s, 1);
    String body = PARG_OTHER(s, 1);
    ws.send_secret(0x01, body, 0, message_id);
    return EMPTY_STRING;
}

/// отправить простой текст и не ждать ответа
/// WS.TEXT
if (cmd == "WS.TEXT") {
    if (!ws.is_connected()) return ERR("WS not connected");
    ws.send(PARG_OTHER(s, 0));
    return EMPTY_STRING;
}

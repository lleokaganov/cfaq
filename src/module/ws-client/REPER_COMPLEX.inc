// {WS.key:0}
if(CMD == F("WS.key")) {
    uint32_t id = (uint32_t) PARG0(l, 0);
    UserKey* key = key_find(id); // серверный ключ имеет user_id = 0
    if(!key) return "";
    return String(id)+"-"+bin2hex(key->x, 32)+"-"+bin2hex(key->ed, 32);
}

/// {WS.SECURE:0x00 12345 0 {"action":"my_info"}}
if (CMD == F("WS.SECURE")) {
    if (!ws.is_connected()) return "";
    uint8_t cmd = (uint8_t)PARG0(l, 0);
    uint32_t to = (uint32_t)PARG0(l, 1);
    uint16_t message_id = (uint16_t)PARG0(l, 2);
    String body = PARG_OTHER(l, 2);
    String out; ws.send_secret_request(out, cmd, body, to, message_id);
    return out;
}

/// {WS.SERVER:{"action":"my_info"}}
if (CMD == F("WS.SERVER")) { // отправить данные без ожидания ответа
    if (!ws.is_connected()) return "";
    String out; ws.send_secret_request(out, 0x00, l, 0);
    return out;
}

/// {WS.DATA:{"action":"my_info"}}
if (CMD == F("WS.DATA")) { // отправить данные без ожидания ответа
    if (!ws.is_connected()) return "";
    String out; ws.send_secret_request(out, 0x10, l, 0);
    return out;
}

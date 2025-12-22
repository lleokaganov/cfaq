#include "h.hpp"
#include "main.h"

LSocketyee ws;

void ws_onMessageText_cb(const String& s) {
    Serial.println("<onMessageText: " + s + ">");
    // CFSET(
    // DOMOTO(CF('WS.onMessageText'), s);
}

void ws_onMessageBin_cb(uint8_t cmd, uint32_t from, uint16_t msg_id, const String& body) {
    Serial.println("<onMessageBin: cmd=" + String(cmd) + ", from=" + String(from) + ", msg_id=" + String(msg_id) + ", body=" + body + ">");

    if(cmd == 0x04) { // read file
        String name = Slash(body);
        String answer;
        if( !is_file(name) ) answer = "ERR: no file";
        else if( filesize(name) > MAX_PACKET_SIZE - 100 ) answer = "ERR: file big";
        else answer = getfile(name);
        ws.send_secret(0x01, answer, from, msg_id);
        return;
    }

    if(cmd == 0x03) { // save file\ncontent
        String name = body.substring(0, body.indexOf("\n"));
        String content = body.substring(body.indexOf("\n") + 1);
        name = Slash(name);
        String answer;
        if(!name.length() || name.indexOf("\n") >= 0) answer = "ERR: filename";
        else if(!content.length()) answer = "ERR: content";
        else answer = ( file_save(name, content) ? "OK" : "ERR: save error" );
        ws.send_secret(0x01, answer, from, msg_id);
        return;
    }

    if(cmd != 0x00) return; // ответы на ответы не присылать!

    // LOGI(LOG_WEB,"AJAX-MOTO: ["+body+"]");
    EchoMOTO=" ";
    MOTO(body,0);
    if (!ws.is_connected()) return;
    if (EchoMOTO.length() < 2) EchoMOTO = " OK"; // нет данных для ответа
    if (EchoMOTO.endsWith("\n")) EchoMOTO.remove(EchoMOTO.length() - 1);
    ws.send_secret(0x01, EchoMOTO.substring(1), from, msg_id);
    EchoMOTO=""; // очистка после отправки
}
void ws_onConnect_cb() {
    Serial.println("@ @ @ WS connected");
    MOTO("play Dm");
    // CFSET(
    // DOMOTO(CF('WS.onConnect'));
}
void ws_onDisconnect_cb() {
    Serial.println("@ @ @ WS disconnected");
    MOTO("play mD");
    // CFSET(
    // DOMOTO(CF('WS.onDisconnect'));
}



#include "h.hpp"

#if defined(ESP8266)
  #include <Hash.h>
#elif defined(ESP32)
  #include "mbedtls/sha1.h"
#endif
#include <base64.h>

volatile bool ans_awaiting = false;
volatile bool ans_ready = false;
volatile bool ans_err = false;
volatile uint32_t ans_to = 0;
volatile uint16_t ans_id = 0;
String ans_body = "";

LSocketyee::LSocketyee() {}

void LSocketyee::onMessageText(std::function<void(const String&)> callback) { onMessageTextCallback = callback; }
void LSocketyee::onMessageBin(std::function<void(uint8_t cmd, uint32_t from, uint16_t msg_id, const String& body)> callback) { onMessageBinCallback = callback; }
void LSocketyee::onConnect(std::function<void()> callback) { onConnectCallback = callback; }
void LSocketyee::onDisconnect(std::function<void()> callback) { onDisconnectCallback = callback; }
void LSocketyee::setTimeout(uint32_t to_ms) { this->timeout_ms = to_ms; }

bool LSocketyee::is_connected() { return wsSocket && wsSocket->connected() && isWS; }

void LSocketyee::stop() {
  Serial.println("===> WS: stop");
  if (wsSocket) {
    if (WiFi.status() == WL_CONNECTED && wsSocket->connected()) {
        wsSocket->write((uint8_t)0x88);
        wsSocket->write((uint8_t)0x00);
        delay(10);
        wsSocket->stop();
    }
    delete wsSocket;
    wsSocket = nullptr;
  }

  if (isWS && onDisconnectCallback) {
    Serial.println("===> WS: Disconnect in FN");
    onDisconnectCallback();
  }
  isWS = false;
}

void LSocketyee::begin(const String& h, uint16_t p, const String& pa) {
  Serial.println("===> WS: begin: " + h + ":" + String(p) + "|" +pa);
  this->stop();
  host = h;
  port = p;
  path = pa;
  isWS = false;
}

void LSocketyee::loop() {

  if( !WiFi.isConnected() ) return;

  if (wsSocket == nullptr) {
    wsSocket = new WiFiClient();
    // last_reconnect = 0;
  }

  if (wsSocket && !wsSocket->connected()) { // Состояние дисконнектв
    if (isWS && onDisconnectCallback) { // Было соединение, теперь разрыв
      // Serial.println("===> WS: Connected->Disconnected in Loop");
      onDisconnectCallback();
    }
    isWS = false;
    if ( millis() - last_reconnect < timeout_ms ) { // Ожидание реконнекта
        // static uint32_t mls = 0;
        // if(mls != millis()) {
        //     mls = millis();
        //     if(millis()%400 == 0) Serial.print(".");
        // }
        return;
    }
    if (wsSocket->connect(host.c_str(), port)) { // Пытаемся подключиться
      Serial.println("===> WS: New connect in Loop");
      String clientKey = "dGhlIHNhbXBsZSBub25jZQ==";
      wsSocket->print("GET " + path + " HTTP/1.1\r\n");
      wsSocket->print("Host: " + host + "\r\n");
      wsSocket->print("Upgrade: websocket\r\n");
      wsSocket->print("Connection: Upgrade\r\n");
      wsSocket->print("Sec-WebSocket-Key: " + clientKey + "\r\n");
      wsSocket->print("Sec-WebSocket-Version: 13\r\n\r\n");
    } else {
      // Serial.println("===> WS: Connect failed in Loop"); // Не удалось, по-прежнему ждем
    }
    last_reconnect = millis();
  }
  
  if (wsSocket && wsSocket->connected() && wsSocket->available()) {
    if(!isWS) {
      String line = wsSocket->readStringUntil('\n');
      // Serial.println("===> WS: Handshake response: " + line);

      if (line.indexOf("101") >= 0) {
        while (wsSocket->available()) wsSocket->read(); // тупо чистим буфер
        isWS = true;
        if (onConnectCallback) {
          onConnectCallback();
          // Serial.println("===> WS: Connected in Loop!!!");
        }
      } else {
        // Serial.println("===> WS: Handshake failed");
        wsSocket->stop();
        // delete wsClient;
        // wsClient = nullptr;
      }
    } else { // handleClientFrame
      uint8_t opcode;
      if (!read1(*wsSocket, opcode)) return;
      uint32_t len = readLen(*wsSocket);
      if (len > MAX_PACKET_SIZE) return;
      char* data = (char*)malloc(len + 1);

      uint8_t b;
      for (int i = 0; i < len; i++) {
        if (!read1(*wsSocket, b)) {
            Serial.println("===> Error ws: payload timeout");
            free(data);
            return;
        }
        data[i] = b;
      }

      switch (opcode & 0x0F) {

        case 0x09: { // PING ответ Pong с тем же payload
            wsSocket->write((uint8_t)0x8A); // FIN + Pong opcode
            wsSocket->write(0x80 | (uint8_t)len); // MASK=1 + длина (<=125 нам хватает)
            uint8_t mask[4] = { (uint8_t)random(0,255), (uint8_t)random(0,255), (uint8_t)random(0,255), (uint8_t)random(0,255) };
            wsSocket->write(mask, 4);
            for (size_t i = 0; i < len; i++) wsSocket->write(data[i] ^ mask[i & 3]);
            break;
        }

        case 0x0A: break; // PONG

        case 0x08: { // CLOSE отправить ответный CLOSE
            wsSocket->write((uint8_t)0x88);
            wsSocket->write((uint8_t)0x00);
            wsSocket->stop();
            isWS = false;
            if (onDisconnectCallback) onDisconnectCallback();
            break;
        }

        case 0x01: { // TEXT
            data[len] = '\0';
            String s = String(data);
            // Serial.println("===> WS: Received text: " + s);

            if(s.startsWith("timestamp_error:")) {
              UnixTime = strtoull(s.substring(s.indexOf(':') + 1).c_str(), nullptr, 10);
              // unixtime_remote = strtoull(s.substring(s.indexOf(':') + 1).c_str(), nullptr, 10);
              // unixtime_mark = millis();
            }

            if (onMessageTextCallback) onMessageTextCallback(String(data));
            break;
        }

        case 0x02: { // BINARY
            // Serial.print("===> WS: Received data, len=" + String(len));
            // ============ decoder ============
            if (len < 4 + 8 + 16 + 64) { ans_err = true; ans_body = "ERR_BIN"; break; }

            uint32_t from = (uint32_t)data[0] | ((uint32_t)data[1] << 8) | ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);

            UserKey* keyfrom = key_find(from);
            if (!keyfrom) { ans_err = true; ans_body = "ERR_KEY"; break; }

            const uint8_t *packet = (const uint8_t*)data + 4;
            size_t packet_len = len - 4;

            // максимум: packet_len - 8 - 16 - 64
            int max_plain = packet_len - 8 - 16 - 64;
            if (max_plain <= 0) { ans_err = true; ans_body = "ERR_SIZE";  break; }

            uint8_t *plain = (uint8_t*)malloc(max_plain);
            if (!plain) { ans_err = true; ans_body = "ERR_MEMORY";  break; }

            bool ok = crypto25519_verify_and_decrypt( packet, packet_len,
                x_secret,        // 32
                keyfrom->x, // he_x,     // 32
                keyfrom->ed, // he_ed   // 32
                0,   // now
                0,               // max_nonce_skew
                plain            // output
            );

            if (!ok) { free(plain); ans_err = true; ans_body = "ERR_CRYPTO"; break; }

            // теперь plain содержит:
            // [u16 msg_id LE][u8 cmd][body...]

            if (max_plain < 3) { free(plain); ans_err = true; ans_body = "ERR_BODY"; break; }

            uint16_t msg_id = plain[0] | (plain[1] << 8);
            uint8_t cmd     = plain[2];

            size_t body_len = max_plain - 3;
            String body = "";
            body.reserve(body_len);

            for (size_t i = 0; i < body_len; i++) body += (char)plain[3 + i];

            free(plain);

            // ============ /decoder ===========
            // итог: cmd=0x01, body, from, msg_id

            if (cmd == 0x01 && ans_awaiting && msg_id == ans_id && from == ans_to) {
                ans_body = body;
                ans_ready = true;
                ans_err = false;
                free(data); data = nullptr;
                break;
            }

            // Это был не ответ или не нам
            if(body == "ping") { ws.send_secret(0x01, "pong", from, msg_id); break; }
            if(body == "pong") break;

            if (onMessageBinCallback) onMessageBinCallback(cmd, from, msg_id, body);
            // Serial.println(
            //   "##########################################################"
            //   "\n# cmd: " + String(cmd) +" == 1" +
            //   "\n# msg_id: " + String(msg_id) +" == " +String(ans_id) +
            //   "\n# from: " + String(from) +" == " +String(ans_to) +
            //   "\n# ans_awaiting: " + String(ans_awaiting) +
            //   "\n##########################################################"
            // );

            break;
        }

        default: break; // ignore
      } // switch
      if (data != nullptr) free(data);
    }
  }
}

void LSocketyee::writeLen(WiFiClient& sock, size_t len, uint8_t mask) {
  if (len <= 125) {
    sock.write(len | mask);
  } else if (len <= 65535) {
    sock.write(126 | mask);
    sock.write((len >> 8) & 0xFF);
    sock.write(len & 0xFF);
  } else {
    sock.write(127 | mask);
    for (int i = 7; i >= 0; i--) sock.write((len >> (8 * i)) & 0xFF);
  }
}

void LSocketyee::send(const String& text) {
  if (!is_connected()) return;
  size_t len = text.length();
  // длина заголовка максимум: 2 (opcode + length) + 8 (extended len) + 4 (mask)
  uint8_t header[14];
  size_t i = 0;
  // первый байт: FIN + text
  header[i++] = 0x81;
  // маска
  uint8_t mask[4] = { (uint8_t)random(0,255), (uint8_t)random(0,255), (uint8_t)random(0,255), (uint8_t)random(0,255) };
  // длина
  if (len <= 125) {
    header[i++] = 0x80 | (uint8_t)len;
  } else if (len <= MAX_PACKET_SIZE) { // 65535
    header[i++] = 0x80 | 126;
    header[i++] = (len >> 8) & 0xFF;
    header[i++] = len & 0xFF;
  } else {
    Serial.println("===> WS: Message too long to send: " + String(len) );
    return;
  }
  // маска
  memcpy(header + i, mask, 4);
  i += 4;
  // общий буфер: заголовок + данные
  uint8_t* frame = (uint8_t*)malloc(i + len);
  memcpy(frame, header, i);
  // XOR
  for (size_t j = 0; j < len; j++) frame[i + j] = text[j] ^ mask[j % 4];
  wsSocket->write(frame, i + len);
  free(frame);
}

void LSocketyee::send_bin(const uint8_t* data, size_t len) {
  if (!is_connected()) return;
  // заголовок максимум:
  // 2 (FIN+opcode + length) + 8 (extended len) + 4 (mask)
  uint8_t header[14];
  size_t i = 0;
  // FIN=1, opcode=2 (binary)
  header[i++] = 0x82;
  // маска (клиент обязан маскировать)
  uint8_t mask[4] = { (uint8_t)random(0,255), (uint8_t)random(0,255), (uint8_t)random(0,255), (uint8_t)random(0,255) };
  // длина
  if (len <= 125) {
    header[i++] = 0x80 | (uint8_t)len; // MASK=1 + len
  } else if (len <= MAX_PACKET_SIZE) { // 65535
    header[i++] = 0x80 | 126;
    header[i++] = (len >> 8) & 0xFF;
    header[i++] = len & 0xFF;
  } else {
    Serial.println("===> WS: Binary too long");
    return;
  }
  memcpy(header + i, mask, 4);
  i += 4;
  // общий фрейм
  uint8_t *frame = (uint8_t*)malloc(i + len);
  memcpy(frame, header, i);
  // XOR payload
  for (size_t j = 0; j < len; j++) {
    frame[i + j] = data[j] ^ mask[j % 4];
  }
  wsSocket->write(frame, i + len);
  free(frame);
}

bool LSocketyee::read1(WiFiClient& sock, uint8_t& out, uint32_t timeout) {
    uint32_t start = millis();
    while (sock.connected()) {
        if (sock.available()) { out = sock.read(); return true; }
        if (millis() - start > timeout) return false;
        delay(0);
    }
    return false;
}

uint32_t LSocketyee::readLen(WiFiClient& sock) {
    uint8_t l;
    if (!read1(sock, l)) return 0;
    l &= 0x7F;
    if (l <= 125) return l;
    if (l == 126) {
        uint8_t h;
        if (!read1(sock, h) || !read1(sock, l)) return 0;
        return ((uint32_t)h << 8) | (uint32_t)l;
    }
    return 0;
}

uint16_t LSocketyee::next_id() {
    static uint16_t id = 1;
    id++;
    if (id == 0) id = 1;
    return id;
}

void LSocketyee::send_secret(uint8_t cmd, const String& body, uint32_t to, uint16_t message_id) {
  if (!is_connected()) return;
  if (message_id == 0) {
      message_id = next_id();
  }

  size_t body_len = body.length();

  // 1) Сборка inner
  // inner = [u16 msg_id LE][u8 cmd][body...]
  size_t inner_len = 2 + 1 + body_len;
  uint8_t inner[inner_len];

  inner[0] = message_id & 0xFF;
  inner[1] = (message_id >> 8) & 0xFF;
  inner[2] = cmd;
  memcpy(inner + 3, body.c_str(), body_len);

  // 2) Шифруем и подписываем
  // packet = [8 байт nonce][cipher(len + 16)][64 sig]
  size_t enc_max = 8 + (inner_len + 16) + 64;
  uint8_t encrypted[enc_max];

  UserKey* keyto = key_find(to);
  if (!keyto) {
    Serial.println("===> WS: No key for id " + String(to));
    return;
  }

  // const uint8_t* x_he_public  = keyto->x;
  // const uint8_t* ed_he_public = keyto->ed;

  crypto25519_encrypt_and_sign(
      inner,
      inner_len,
      x_secret,       // 32 байта
      ed_secret,      // 64 байта
      keyto->x,       // server_x,    // 32 байта
      encrypted       // итоговый пакет
  );

  // 3) Формируем финальный payload:
  // payload = [u32 to LE][encrypted...]
  size_t encrypted_len = 8 + (inner_len + 16) + 64;
  uint8_t payload[4 + encrypted_len];

  payload[0] = (uint8_t)(to);
  payload[1] = (uint8_t)(to >> 8);
  payload[2] = (uint8_t)(to >> 16);
  payload[3] = (uint8_t)(to >> 24);

  memcpy(payload + 4, encrypted, encrypted_len);

  // 4) Отправка в WebSocket
  send_bin(payload, 4 + encrypted_len);
}

bool LSocketyee::send_secret_request(String& out_response, uint8_t cmd, const String& body, uint32_t to, uint32_t timeout_ms) {
    if (!is_connected()) return false;
    uint16_t id = next_id();

    ans_id = id;
    ans_to = to;
    ans_ready = false;
    ans_awaiting = true;
    // ans_err = false;

    // тут ты формируешь пакет: msg_id=id, cmd=0x00, body
    send_secret(cmd, body, to, id);

    if (!waitForResponse(out_response, 1500)) {
        ans_awaiting = false;
        return false;
    }
    ans_awaiting = false;
    return true;
}

bool LSocketyee::waitForResponse(String& out, uint32_t timeout_ms) {
    uint32_t start = millis();
    while (millis() - start < timeout_ms) {
      if (ans_ready) {
        out = ans_body;
        if(ans_err) return false;
        return true;
      }
      loop();
      delay(1); // yield
    }
    return false;
}

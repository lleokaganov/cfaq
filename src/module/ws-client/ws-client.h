#pragma once
#include "h.hpp"

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #define MAX_PACKET_SIZE 4096
#elif defined(ESP32)
  #include <WiFi.h>
  #define MAX_PACKET_SIZE 8192
#endif

#include <functional>

class LSocketyee {
  public:
    LSocketyee();

    void begin(const String& host, uint16_t port, const String& path = "/");
    void stop();
    bool is_connected();

    void onMessageText(std::function<void(const String&)> callback);
    void onMessageBin(std::function<void(uint8_t cmd, uint32_t from, uint16_t msg_id, const String& body)> callback);

    void onConnect(std::function<void()> callback);
    void onDisconnect(std::function<void()> callback);

    void send(const String& text);
    void send_bin(const uint8_t* data, size_t len);
    void loop();
    void setTimeout(uint32_t timeout_ms);

    String cryptomessage_decode(const uint8_t* data, size_t len);

    String host;
    uint16_t port;
    String path;

    bool send_secret_request(String& out_response, uint8_t cmd, const String& body, uint32_t to, uint32_t timeout_ms = 2000);
    void send_secret(uint8_t cmd, const String& body, uint32_t to, uint16_t message_id = 0);

  private:
    WiFiClient* wsSocket = nullptr;

    bool isWS = false;

    std::function<void(const String&)> onMessageTextCallback;
    std::function<void(uint8_t cmd, uint32_t from, uint16_t msg_id, const String& body)> onMessageBinCallback;
    std::function<void()> onConnectCallback;
    std::function<void()> onDisconnectCallback;

//    void handle();
//    void handleFrame();

    void writeLen(WiFiClient& sock, size_t len, uint8_t mask);
    uint32_t readLen(WiFiClient& sock);
    bool read1(WiFiClient& sock, uint8_t& out, uint32_t timeout=2000);

    uint32_t timeout_ms = 5000;
    uint32_t last_reconnect;

    uint16_t next_id();
    bool waitForResponse(String& out, uint32_t timeout_ms);
};

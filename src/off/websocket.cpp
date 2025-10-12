#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>
using namespace websockets;

const char* ssid = "D922";
const char* password = "77777701";

const char* ws_server = "10.1.1.8";
const int ws_port = 8099;
const char* ws_path = "/ws";

WebsocketsClient wsclient;

// unsigned long lastConnectAttempt = 0;

void onMessageCallback(WebsocketsMessage message) {
  Serial.printf("Got Message: %s\n", message.data().c_str());
}

void onEventCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("WebSocket Connected!");
    wsclient.send("Hello from ESP32!");
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("WebSocket Disconnected!");
  } else if (event == WebsocketsEvent::GotPing) {
    Serial.println("Got a Ping!");
  } else if (event == WebsocketsEvent::GotPong) {
    Serial.println("Got a Pong!");
  }
}

void connectWebSocket() {
  Serial.println("Connecting to WebSocket...");
  bool connected = wsclient.connect(ws_server, ws_port, ws_path);
  if (connected) {
    Serial.println("WebSocket connection established.");
    wsclient.onMessage(onMessageCallback);
    wsclient.onEvent(onEventCallback);
  } else {
    Serial.println("WebSocket connection failed!");
  }
}

// --------------------------------------------------------------

// put function declarations here:
// int myFunction(int, int);

void setup() {
  // int result = myFunction(2, 3);
  Serial.begin(115200);
  Serial.println("Result: ");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  connectWebSocket();
}

unsigned long xtime = 0;

static unsigned long ws_lastPing = 0;
static unsigned long ws_lastConnectAttempt = 0;

void loop() {

  if(millis() - xtime >= 1000) {
    xtime = millis();
    Serial.println("Time: "+String(xtime));
  }

  delay(10);

  if (wsclient.available()) {
    wsclient.poll();
  } else {
    if (millis() - ws_lastConnectAttempt > 5000) {  // каждые 5 сек пробуем переподключиться
      connectWebSocket();
      ws_lastConnectAttempt = millis();
    }
  }

  if (millis() - ws_lastPing > 10000 && wsclient.available()) {
    wsclient.ping();
    ws_lastPing = millis();
  }
}

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }
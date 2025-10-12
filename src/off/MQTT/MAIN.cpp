// MQTT MAIN:

#include <PubSubClient.h>
WiFiClient MQTT_espClient;
PubSubClient client(MQTT_espClient);
uint8_t MQTT = 0;
// SSL Port  22399
// Websockets Port (TLS only)  32399

// Функция получения данных от сервера
void MQTT_callback(char* topic, byte* payload, unsigned int l) {
  String s = ""; for (int i = 0; i < l; i++) s += (char)payload[i];
  LOGI(LOG_CONN, "MQTT get [" + String(topic) + "]: " + s);
  CFSET("MQTT_TOPIC", String(topic));
  CFSET("MQTT_DATA", s);
  MOTO("/MQTT");
}

/*
  void MQTT_callback(const MQTT::Publish& pub) {
    String s = pub.payload_string();
    LOGI(LOG_CONN,"MQTT: "+pub.topic()+" => "+s);
    if(String(pub.topic()) == "test/led") LOGI(LOG_CONN,"topic = test/led !!!");
  }
*/

//   randomSeed(micros());
// PubSubClient client(wclient, mqtt_server, mqtt_port);
// /MQTT

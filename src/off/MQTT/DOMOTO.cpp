// ---------- MQTT ------------

  if(cmd=="MQTT.connect") { LOGI(LOG_CONN,"Connecting to MQTT server");
    if(MQTT && client.connected()) { iecho("MQTT already connected"); return ""; }

    String c=PARG(s,1); if(c=="") { LOGI(LOG_CONN,"Error MQTT server"); return ""; }
    uint16_t port=PARG0(c,1,":"); if(!port) port=1883; else c=PARG(c,0,":"); // умолчанию 1883 порт (8883 при подключении через SSL). Читать ещё
    CFSET("MQTT_server",c); CFSET("MQTT_port",String(port));

    String login=PARG(s,2); if(login!="") { CFSET("MQTT_login",login); CFSET("MQTT_password",REPER(ARG_OTHER(s,2))); }

    if(CF("MQTT_client")="") CFSET("MQTT_client", REPER("{soft}-{chip}") );

    cmd="MQTT.reconnect";
    // MQTT_reconnect("tailor.cloudmqtt.com","12399","skikwfox","Y5zweNWxQ0E1");
  }

  if(cmd=="MQTT.reconnect") { // https://api.cloudmqtt.com/console/82652968/log
    if(client.connected()) return "";
    client.setCallback(MQTT_callback);
    if(CF("MQTT_server")=="") { ERR("ERROR: Server not set"); return ""; }

     LOGI(LOG_CONN,"MQTT.reconnect server=["+CF("MQTT_server")+"]:["+String(CF0("MQTT_port"))+"]");
//     client.setServer(CF("MQTT_server").c_str(),CF0("MQTT_port"));

// client.setServer("tailor.cloudmqtt.com",12399);
// client.setServer(CF("MQTT_server").c_str(),12399);

 // const char *str = "tailor.cloudmqtt.com";
 // const char *str = CF("MQTT_server").c_str();

  // String str0=CF("MQTT_server");
/*
  unsigned char* buf = new unsigned char[256];
  CF("MQTT_server").getBytes(buf, 256, 0);
  const char *str = (const char*)buf;
*/

  char* str = static_cast<char*>(malloc(CF("MQTT_server").length() + 1));
  memcpy(str, CF("MQTT_server").c_str(), CF("MQTT_server").length() + 1);

  client.setServer(str,CF0("MQTT_port"));

// LOGI(LOG_CONN,"1: [tailor.cloudmqtt.com]\n2: ["+String(CF("MQTT_server").c_str())+"]\nITOG: "+( String(CF("MQTT_server").c_str())=="tailor.cloudmqtt.com"?"YES":"NO") );

    bool i = (CF("MQTT_login")=="" ? client.connect(CF("MQTT_client").c_str()) :  client.connect(CF("MQTT_client").c_str(),CF("MQTT_login").c_str(),CF("MQTT_password").c_str()) );

    if(i) {
        MQTT=1; return "";
        LOGI(LOG_CONN,"MQTT.reconnect CONNECTED");
        }
    MQTT=0;
    LOGI(LOG_CONN,"MQTT.reconnect ERROR: "+String(client.state()) );

    ERR("ERROR "+String(client.state()) ); return "";
  }

  if(cmd=="MQTT.publish") { // Функция публикации MQTT.publish [topick] [string]
    if(MQTT) { client.publish(PARG(s,1).c_str(),REPER(ARG_OTHER(s,1)).c_str()); iecho("OK"); } else iecho("NO MQTT"); return "";
  }

  if(cmd=="MQTT.subscribe") { // Функция подписки MQTT.subscribe [topick]
    if(MQTT) { client.subscribe(PARG(s,1).c_str()); iecho("OK"); } else iecho("NO MQTT"); return "";
  }

  if(cmd=="MQTT.unsubscribe") { // Функция отписки MQTT.unsubscribe [topick]
    if(MQTT) { client.unsubscribe(PARG(s,1).c_str()); iecho("OK"); } else iecho("NO MQTT"); return "";
  }

// ---------- /MQTT ------------

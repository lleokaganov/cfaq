#include <Arduino.h>
#include "fileget.h"

String CHURL(String url) { url.replace("<<","{"); url.replace(">>","}"); return REPER(url); }

String file_get_contents(String url) { return file_get_contents(url, 3000); }
String file_get_contents(String url, uint16_t timeout) { return file_fetch_contents("GET", url, "", timeout, ""); }
String file_post_contents(String url, String data) { return file_post_contents(url, data, 3000); }
String file_post_contents(String url, String data, uint16_t timeout) { return file_post_contents(url, data, timeout, ""); }
String file_post_contents(String url, String data, uint16_t timeout, String mode) { return file_fetch_contents("POST", url, data, timeout, mode); }

String file_fetch_contents(String method, String url, String data, uint16_t timeout, String mode) {
    if(!ISNET) return ""; // если нет подключения к сети
    url = CHURL(url);

    HTTPClient http;
    http.setTimeout(timeout);

#ifdef ESP32
    // Выбираем HTTP или HTTPS
    WiFiClientSecure wificlient_secure;
    WiFiClient wificlient;
    if(url.startsWith("https:")) {
        wificlient_secure.setInsecure(); // Отключаем проверку сертификатов (НЕБЕЗОПАСНО, но работает)
        http.begin(wificlient_secure, url.c_str());
    } else {
        http.begin(wificlient, url.c_str());
    }
#else
    WiFiClient wificlient;
    http.begin(wificlient, url.c_str());
#endif

    // Добавляем заголовки (JSON)
    if(mode=="JSON") {
	http.addHeader("Content-Type", "application/json");
	data.trim();
	if(data.length() && data[0] != '{') data = "{"+data+"}"; // ну хоть такой грязный патч }
    }

    // включить сбор заголовков
    const char* headers[] = {"Content-Type"};  // Объявляем массив заголовков
    http.collectHeaders(headers, 1);  // Передаём его в collectHeaders()

    // Отправляем запрос
    int c;
    if(method=="GET") c = http.GET();
    else c = http.POST(data);

    LOGI(LOG_WGET, "file_"+method+"_contents: [" + url + "] timeout=" + String(timeout));

    String s = "";
    String contentType = "";

    if(c == HTTP_CODE_OK) {
        contentType = http.header("Content-Type");
	s = http.getString();
    } else if (LOGLI(LOG_WGET)) Serial.printf("\terror #%d: %s (timeout=%d)\n", c, http.errorToString(c).c_str(), timeout);

    http.end();

    // if(mode=="JSON")
    if(contentType.indexOf("application/json") >= 0) { s.replace("{","<"); s.replace("}",">"); } // блять, какое извращение

///
// LOGI(LOG_WGET, " @@@ --> fetch.ok");

    return s;
}

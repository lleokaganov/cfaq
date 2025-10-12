#include "esp_camera.h"
// #include <TJpg_Decoder.h>


// #define FARG_TAB(key) \
//  FARGTAB(s, #key, key##_tb, sizeof(key##_tb)/sizeof(key##_tb[0]))

//  static const FargMap name##_tb[] = { __VA_ARGS__ };

    static const FargMap size_tb[] = {
        {"QVGA", FRAMESIZE_QVGA},       {"320x240", FRAMESIZE_QVGA},
                                        {"96x96", FRAMESIZE_96X96},
                                        {"240x240",FRAMESIZE_240X240},
        {"QQVGA", FRAMESIZE_QQVGA},     {"160x120", FRAMESIZE_QQVGA},
        {"CIF", FRAMESIZE_CIF},         {"352x288", FRAMESIZE_CIF},
        {"VGA", FRAMESIZE_VGA},         {"640x480", FRAMESIZE_VGA},
        {"SVGA", FRAMESIZE_SVGA},       {"800x600", FRAMESIZE_SVGA},
        {"XGA", FRAMESIZE_XGA},         {"1024x768", FRAMESIZE_XGA},
        {"SXGA", FRAMESIZE_SXGA},       {"1280x1024", FRAMESIZE_SXGA},
        {"UXGA", FRAMESIZE_UXGA},       {"1600x1200", FRAMESIZE_UXGA},
        {"QCIF", FRAMESIZE_QCIF},       {"176x144", FRAMESIZE_QCIF},
        {"HQVGA", FRAMESIZE_HQVGA},     {"240x160", FRAMESIZE_HQVGA},
        {"HD", FRAMESIZE_HD},           {"1280x720", FRAMESIZE_HD},
        {"FHD", FRAMESIZE_FHD},         {"1920x1080", FRAMESIZE_FHD},
        {"PHD", FRAMESIZE_P_HD},        {"720x1280", FRAMESIZE_P_HD},
        {"PFHD", FRAMESIZE_P_FHD},      {"1080x1920", FRAMESIZE_P_FHD},
        {"QXGA", FRAMESIZE_QXGA},       {"2048x1536", FRAMESIZE_QXGA},
        {"HVGA",FRAMESIZE_HVGA},        {"480x320", FRAMESIZE_HVGA},
        {"P3MP",FRAMESIZE_P_3MP},       {"864x1536", FRAMESIZE_P_3MP},
        {"QHD", FRAMESIZE_QHD},         {"2560x1440", FRAMESIZE_QHD},
        {"WQXGA",FRAMESIZE_WQXGA},      {"2560x1600", FRAMESIZE_WQXGA},
        {"QSXGA",FRAMESIZE_QSXGA},      {"2560x1920", FRAMESIZE_QSXGA}
    };

uint8_t camera_init_flag=0; // инициализирована ли камера

uint8_t camera2tft = 0; // Интервал между кадрами в миллисекундах
uint32_t camera2tft_last = 0; // Время последнего кадра

String camera2server(camera_fb_t* fb, String url) {
  if(!fb || fb->len == 0) return "Error: empty";
  if(!url.startsWith("https://")) return "Error: https:// need";
  String hostAndPath = url.substring(8); // убираем https://
  int slashIndex = hostAndPath.indexOf('/');
  if(slashIndex == -1) return "Error: path";
  String host = hostAndPath.substring(0, slashIndex);
  String path = hostAndPath.substring(slashIndex);
  WiFiClientSecure client;
  client.setInsecure();
  client.setTimeout(2);  // таймаут 2 сек
  if(!client.connect(host.c_str(), 443)) return "Errror: connection";
  // Заголовки
  client.println("POST "+path+" HTTP/1.1");
  client.println("Host: "+host);
//  client.println("User-Agent: ESP32-CAM");
  client.println("Connection: close");
//  client.println("Content-Type: image/jpeg");
//  client.println("Accept-Encoding: identity");
  client.println("Content-Length: "+String(fb->len));
  client.println(); // пустая строка перед телом
  client.write(fb->buf, fb->len);
  // Чтение ответа
  while(client.connected()) { String line = client.readStringUntil('\n'); if(line == "\r") break; }
  String res = "";
  while(client.available()) {
      String lenStr = client.readStringUntil('\n');
      int chunkLen = (int) strtol(lenStr.c_str(), nullptr, 16);
      if (chunkLen == 0) break;
      String chunk = client.readStringUntil('\n');
      res += chunk;
      // String res = ""; while(client.available()) res += (res==""?"":"\n")+client.readStringUntil('\n');
  }
  client.stop();
  return res;
}

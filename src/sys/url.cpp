#include <Arduino.h>
#include "main.h"

url_struct url_parse(String s) {
  url_struct u;

  // https
  int protoPos = s.indexOf("://");
  if (protoPos >= 0) {
    u.protocol = s.substring(0, protoPos);
    s.remove(0, protoPos + 3);
  } else {
    u.protocol = "https"; // по умолчанию https
  }

  int colon = s.lastIndexOf(':');
  int slash = s.indexOf('/');

  if (colon >= 0 && (slash < 0 || colon < slash)) {
    // Есть порт
    u.host = s.substring(0, colon);
    u.port = s.substring(colon + 1, ( slash >= 0 ? slash : s.length()) ).toInt();
  } else {
    // Нет порта
    u.host = (slash < 0 ? s : s.substring(0, slash) );
    u.port = (u.protocol == "wss" || u.protocol == "https") ? 443 : 80;
  }
  u.path = ( slash < 0 ? "/" : s.substring(slash) );
  return u;
}


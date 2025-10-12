#include <Arduino.h>
// #include <MD5Builder.h>
#include "mainutils.h"

String MD5(String s) {
  MD5Builder md5;
  md5.begin();
  md5.add(s.c_str());
  md5.calculate();
  return md5.toString();
}

String njsn(String s) {
  s.replace("\n", "");
  s.replace("\r", "");
  s.replace("\"", "\\\"");
  return s;
}
String njs(String s) {
  s.replace("\n", "\\n");
  s.replace("\r", "");
  s.replace("\"", "\\\"");
  return s;
}
String h(String s) {
  s.replace("<", "&lt;");
  s.replace(">", "&gt;");
  return s;
}

String FullInfo() {
  String s = REPERFILE("/info.txt");
  if (s == "") s = REPER(InfoTemplate);
  return s;
}



void SYSP(String s) {
  if (CF("sound.pin") != "") DOMOTO("play " + s);
}

void iecho(String s) {
  if (EchoMOTO.length() != 0 && (EchoMOTO.length() + s.length()) < 4000) EchoMOTO += s;
  LOGI(LOG_IECHO, s);
}

String urlencode(String s) {
  String o = "";
  char c, c0;
  for (uint16_t i = 0; i < s.length(); i++) {
    c = s.charAt(i);
    if (c == ' ') o += '+';
    else if (isalnum(c) || c == '-' || c == '_' || c == '.') o += c;
    else {
      c0 = c & 0x0F;
      c0 = (c0 > 9 ? c0 - 10 + 'A' : c0 + '0');
      c = (c >> 4) & 0x0F;
      c = (c > 9 ? c - 10 + 'A' : c + '0');
      o += '%';
      o += c;
      o += c0;
    }
  }
  return o;
}



// ====================================================
String D00(byte x) { return String(x < 10 ? "0" : "") + String(x); }
String Time_hh(uint32_t T) { return D00( (((T % 86400L) / 3600 ) + CF("timezone", "+3").toInt() ) % 24 ); }
String Time_mm(uint32_t T) { return D00( (T % 3600) / 60 ); }
String Time_ss(uint32_t T) { return D00( T % 60 ); }
String Time_dn(uint32_t T) { return String( ( ( ( (T / 3600 ) + CF("timezone", "+3").toInt() ) / 24 ) + 4 ) % 7 ); } // день недели 0-6
String Time_ddn(uint32_t T) { // дни недели su mo tu we th fr sa
  uint8_t l = Time_dn(T).toInt();
  if(!l) return "su";
  if(l==1) return "mo";
  if(l==2) return "tu";
  if(l==3) return "we";
  if(l==4) return "th";
  if(l==5) return "fr";
  return "sa";
}
String Time_hhmmss(uint32_t T) { return Time_hh(T) + ":" + Time_mm(T) + ":" + Time_ss(T); }

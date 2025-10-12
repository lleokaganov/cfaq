#include <Arduino.h>
#include "ARG.h"

uint16_t findkey(String S, String code) { // найти строку по первому слову, вернуть указатель
  int i = ("\n" + S).indexOf("\n" + code + " "); return (i < 0 ? 0xFFFF : i);
}

uint16_t findstring(String S, String code) { // найти заданную строку, вернуть указатель на нее
  int i = ("\n" + S + "\n").indexOf("\n" + code + "\n"); return (i < 0 ? 0xFFFF : i);
}

bool PARGB(String s, int i, bool def) {
  String c = PARG(s, i); c.toUpperCase();
  if (c == "") return def;
  if (c == "YES" || c == "TRUE") return true;
  if (c == "NO" || c == "FALSE" || c == "STOP") return false;
  return (c.toInt() ? true : false);
}
bool PARGB(String s, int i) { return PARGB(s, i, false); }
String PARG(String s, int n, String sep) { return REPER(ARG(s, n, sep)); }
String PARG(String s, int n) { return PARG(s, n, " "); }
int PARG0(String s, int n, String sep, int def) { String c = PARG(s, n, sep); if(c=="") return def; return (c.indexOf("{") < 0 ? c.toInt() : def); }
int PARG0(String s, int n, String sep) { String c = PARG(s, n, sep); return (c.indexOf("{") < 0 ? c.toInt() : 0); }
int PARG0(String s, int n) { return PARG0(s, n, " "); }
double DPARG0(String s, int n, String sep) { String c = PARG(s, n, sep); return (c.indexOf("{") < 0 ? c.toDouble() : 0); }
double DPARG0(String s, int n) { return DPARG0(s, n, " "); }


String FARG(String s, String name, String def) {
    int start = 0;
    while(start < s.length()) {
        int end = s.indexOf(' ',start);
        if(end == -1) end = s.length();
        String token = s.substring(start, end);
        int eq = token.indexOf('=');
        // if(eq > 0 && token.substring(0, eq) == name) return token.substring(eq+1);
	if(eq > 0 && token.substring(0, eq).equals(name)) {
	    String res = token.substring(eq+1);
	    LOGI(LOG_ERROR, "FARG ["+name+"]=["+res+"]");
	    return res;
	}
        start = end+1;
    }
    return def;
}
String FARG(String s, String name) { return FARG(s,name,""); }
int FARG0(String s, String name, int def) { String c=FARG(s,name,String(def)); if(c=="") return def; return (c.indexOf("{") < 0 ? c.toInt() : def); } // }
int FARG0(String s, String name) { return FARG0(s,name,0); }

int FARGTAB(String s, String key, const FargMap* table, int tableSize) {
  String val = FARG(s,key,"");
  for(int i=0; i<tableSize; i++) { if(val == table[i].name) return table[i].value; }
  return table[0].value;  // Значение по умолчанию — первый элемент
}


String ARG(String s, int n, String sep) {
  int i = 0;
  for(int j = 0; j < n; j++) {
    i = s.indexOf(sep, i) + 1;
    if(i <= 0) return "";
  } return s.substring(i, s.indexOf(sep, i));
}
String ARG(String s, int n) {  return ARG(s, n, " "); }

String ARG_OTHER(String s, int n, String sep) {
  int i = 0;
  for (int j = 0; j <= n; j++) {
    i = s.indexOf(sep, i) + 1;
    if (i <= 0) return "";
  } return s.substring(i);
}
String ARG_OTHER(String s, int n) { return ARG_OTHER(s, n, " "); }
String PARG_OTHER(String s, int n, String sep) { return REPER(ARG_OTHER(s, n, sep)); }
String PARG_OTHER(String s, int n) { return REPER(ARG_OTHER(s, n, " ")); }

uint16_t count(String s, String c) {
  int16_t i = 0;
  uint16_t k = 1;
  while (1) {
    i = s.indexOf(c, i);
    if (i++ < 0) break;
    k++;
  } return k;
}
uint16_t count(String s) {
  return count(s, " ");
}

String nocomments(String s) {
  int j = s.indexOf("#");
  if (j >= 0) {
    s = s.substring(0, j);
    s.trim();
  } return s;
}

bool CFSET(String n, String v) {
  if (n == "") {
    LOGI(LOG_ERROR, "Error CFSET #0 var=[" + v + "]");
    return 0;
  }
  for (uint8_t i = 0; i < NSET; i++) {
    if (Settes[i].n == "") {
      Settes[i].n = n;  // если найдена свободная ячейка
      Settes[i].v = v;
      return 1;
    }
    if (Settes[i].n == n) {
      Settes[i].v = v;  // если найдено это имя
      return 1;
    }
  }
  LOGI(LOG_ERROR, "Error SET: no room"); return 0;
}

String CF(String v, String D) {
  if (v == "") {
    LOGI(LOG_ERROR, "Error CF #0 var=[" + D + "]");
    return "";
  }
  for (uint8_t i = 0; i < NSET; i++) {
    if (Settes[i].n == "") break;
    if (Settes[i].n == v) return Settes[i].v;
  } return D;
}

/// char* CFs(String v) { return CF(v,"").c_str(); }
int CF0(String v, int D) {
  String c = CF(v, "");
  c.toUpperCase();
  if (c == "NO" || c == "") c = "0";
  else if (c == "YES" || c == "TRUE") c = "1";
  return (c == "" ? D : c.toInt());
}
String CF(String v) {
  return CF(v, "");
}
int CF0(String v) {
  return CF0(v, 0);
}
double DCF0(String v) {
  return CF(v, "").toDouble();
}

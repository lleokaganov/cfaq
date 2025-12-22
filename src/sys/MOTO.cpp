#include <Arduino.h>
#include "main.h"

uint16_t jsearch(String S) {
  for (uint16_t zi = 0, i = 0; i < S.length(); i++) {
    if (S[i] == '}') {
      if (!zi) return i;
      else zi--;
    } else if (S[i] == '{') zi++;
  }
  LOGI(LOG_ERROR, "ERROR [" + S + "]");
  return S.length();
}

String MOTO(String s) { return MOTO(s, 1); }
String MOTO(String s, byte yild) {
  if(s == "") return "";
  if(s.indexOf("\n") < 0) s.replace(";","\n");

  s.replace("{ ", "{\n");

  bool IF = 0;
  int16_t i = 0, j = 0;
  String ANS = "NO";
  String S = "";
  String other = "";
  String cmd = "";

  while (i < s.length()) {
    if(yild) yield(); // давать дышать

    j = s.indexOf("\n", i);
    S = ( j < 0 ? s.substring(i) : s.substring(i, j) );

    i = (j < 0 ? s.length() : j + 1); // i всегда теперь указывает на следующую строчку или в конец файла, если следующей строчки нет
    S = nocomments(S); S.replace("\t", " "); S.replace("\r", ""); S.trim(); if (S == "") continue;

    ANS = DOMOTO(S, yild);

    // ---
    if (ANS != "") {
      cmd = ARG(ANS, 0);
      other = ARG_OTHER(ANS, 0); // аргумент до конца строки
      if (ARG(ANS, 1) == "{") {
        j = i + jsearch(s.substring(i));  // аргумент в фигурных скобках
        other = s.substring(i, j);
        i = j + 1;
      }

      if (cmd == "FALSE") {
        IF = 0;
        continue;
      }
      if (cmd == "TRUE" || (cmd == "else" && !IF) ) {
        String o = MOTO(other, yild);
        if (o != "") return o;
        IF = 1;
        continue;
      }

      if (cmd == "LOOP") { // LOOP [повтор7 номер1 секунд30 ВЫПОЛНЕНИЕ]
        uint32_t value = PARG0(other, 2);
        uint8_t res;
        if (value) {
          String ot = ARG_OTHER(other, 2); if (ot == "{") {
            j = i + jsearch(s.substring(i));
            ot = s.substring(i, j);
            i = j + 1;
          }
          res = loop_new( PARG0(other, 0), PARG0(other, 1), value, 0, ot);
        } else {
          String ot = ARG_OTHER(other, 1); if (ot == "{") {
            j = i + jsearch(s.substring(i));
            ot = s.substring(i, j);
            i = j + 1;
          }
          res = loop_new( PARG0(other, 0), 255, PARG0(other, 1), 0, ot);
        }
        if (255 == res) LOGI(LOG_ERROR, "ERROR: loop_new");
        continue;
      }

      if (cmd == "LOOPFILE") { CFSET("MAINLOOP", other); continue; }

      if (cmd == "set") { String Name = PARG(other, 0);
        if(ARG(other,1)=="{") { j=i+jsearch(s.substring(i)); other=s.substring(i,(s[j-1]==32||s[j-1]==10?j-1:j));i=j+1; }
        else other = ARG_OTHER(other, 0);
        CFSET(Name,other); continue;
      }

      if (cmd == "FTSAVE") {
        other.replace("\\n", "\n");
        cmd = "FSAVE";
      }
      if (cmd == "FTSAVE+") {
        other.replace("\\n", "\n");
        cmd = "FSAVE+";
      }
      if (cmd == "FSAVE") {
        String file = PARG(other, 0); if (ARG(other, 1) == "{") {
          j = i + jsearch(s.substring(i));
          other = s.substring(i, (s[j - 1] == 32 || s[j - 1] == 10 ? j - 1 : j));
          i = j + 1;
        }
        else other = ARG_OTHER(other, 0);
        iecho(file_save(file, other) ? "OK" : "ERROR"); continue;
      }
      if (cmd == "FSAVE+") {
        String file = PARG(other, 0); if (ARG(other, 1) == "{") {
          j = i + jsearch(s.substring(i));
          other = s.substring(i, (s[j - 1] == 32 || s[j - 1] == 10 ? j - 1 : j));
          i = j + 1;
        } else other = ARG_OTHER(other, 0);
        iecho(file_save(file, other, (char*)"a+") ? "OK" : "ERROR"); continue;
      }

      if (cmd == "repeat") {
        uint16_t rep = PARG0(ANS, 1); if (!rep) {
          ERR("REPEAT", other);
          continue;
        }
        other = ARG_OTHER(ANS, 1); if (ARG(ANS, 2) == "{") {
          j = i + jsearch(s.substring(i));
          other = s.substring(i, j);
          i = j + 1;
        }
        for (int16_t l = 0; l < rep; l++) {
          String o = MOTO(other, yild);
          if (o == "break") break;
          if (o != "") return o;
        }
        // IF=1;
        continue;
      }

      if (cmd == "POST") {
        if (!ISNET) continue;
        String url, body;
        uint16_t timeout = PARG0(ANS, 1); if (!timeout) {
          timeout = 2000;
          url = PARG(ANS, 1);
          body = PARG_OTHER(ANS, 1);
        }
        else {
          url = PARG(ANS, 2);
          body = PARG_OTHER(ANS, 2);
        }
        if (ARG(body, 0) == "{") {
          j = i + jsearch(s.substring(i));
          body = s.substring(i, j);
          i = j + 1;
        }
        MOTO(file_post_contents(url, body, timeout), yild);
        // Serial.println("\n POSTED: ["+body+"]");
        continue;
      }

      if (cmd == "exit" || cmd == "stop" || cmd == "break") return cmd; // завершить выполнение

    }
    // ---
  }
  return "";
}

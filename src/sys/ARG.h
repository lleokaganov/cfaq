#ifndef ARG_H
#define ARG_H

#include <Arduino.h>

// #include "main.h"
#include "LOG.h"
// #include "hobot/REPER.h"

uint16_t findkey(String S, String code);
uint16_t findstring(String S, String code);

bool PARGB(String s, int i, bool def);
bool PARGB(String s, int i);

String PARG(String s, int n, String sep, String def);
String PARG(String s, int n, String sep);
String PARG(String s, int n);
int PARG0(String s, int n, String sep, int def);
int PARG0(String s, int n, String sep);
int PARG0(String s, int n);
double DPARG0(String s, int n, String sep, double def);
double DPARG0(String s, int n, String sep);
double DPARG0(String s, int n);

String ARG(String s, int n, String sep);
String ARG(String s, int n);
String ARG_OTHER(String s, int n, String sep);
String ARG_OTHER(String s, int n);
String PARG_OTHER(String s, int n, String sep);
String PARG_OTHER(String s, int n);


#define FARG_TAB(key) \
  FARGTAB(s, #key, key##_tb, sizeof(key##_tb)/sizeof(key##_tb[0]))

#define DEFARG(name, ...) \
  static const FargMap name##_tb[] = { __VA_ARGS__ };

// Универсальная структура таблицы строка → число
struct FargMap {
  const char* name;
  int value;
};

String FARG(String s, String name, String def);
String FARG(String s, String name);
int FARG0(String s, String name, int def);
int FARG0(String s, String name);
int FARGTAB(String s, String key, const FargMap* table, int tableSize);


uint16_t count(String s, String c);
uint16_t count(String s);

String nocomments(String s);

bool CFSET(String n, String v);
String CF(String v, String D);
String CF(String v);
int CF0(String v, int D);
int CF0(String v);
double DCF0(String v);

#endif
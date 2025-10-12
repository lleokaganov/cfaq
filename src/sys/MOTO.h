#ifndef MOTO_H
#define MOTO_H

#include <Arduino.h>
#include "LOG.h"
#include "file.h"
#include "mainutils.h"

// Прототипы функций
uint16_t jsearch(String S);
String MOTO(String s);
String MOTO(String s, byte yild);

#endif  // MTO_H

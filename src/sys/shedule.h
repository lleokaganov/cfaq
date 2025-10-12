#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>
#include "mainutils.h"

// --- Планировщик
void oshedule(String key);
void shedule(void);
extern String SheduleS;

#endif // SCHEDULE_H

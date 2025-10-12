#ifndef PLAY_H
#define PLAY_H

#include <Arduino.h>
#include "LOG.h"
#include "MOTO.h"

#ifdef ESP32
    extern "C" {
        #include "driver/ledc.h"
    }
    #define MAX_PWM_CHANNELS 16
#endif

void play(String x);
void play(String x, byte pin);

#endif

#pragma once
#include "Arduino.h"

extern "C" {
    #include "qrcodegen.h"
}

#include "module/TFT_ST7796/h.hpp"

extern void draw_qr(int x0, int y0, int scale, uint8_t *data, int len);

#pragma once

#ifndef LGFX_USE_V1
    #define LGFX_USE_V1
#endif

#include <LovyanGFX.hpp>
#include "LGFX_dyn.hpp"

// extern ST7796_Config* ST7796_cfg;
extern ST7796_Config ST7796_cfg;
extern LGFX tft;

extern uint16_t last_tft_color;
extern uint16_t get_tft_color0(String s, uint16_t n);
extern uint16_t get_tft_color(String s, uint16_t n);

struct ColorMap {
    const char* name;
    uint16_t value;
};


// #include <functional>

struct TouchMap {
    uint16_t x, y, w, h;
    String value;
    String index;
    std::function<void(String,uint16_t,uint16_t)> fn;
};
extern std::vector<TouchMap> touchMap;
void touch_add(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const String& value, const String& index, std::function<void(String,uint16_t,uint16_t)> fn);
void touch_del(const String& idx);
void touch_clean();
void touch_moto_fn(const String& s, uint16_t x, uint16_t y);
void touch_key_fn(const String& s, uint16_t x, uint16_t y);
void touch_keyboard(String layout);
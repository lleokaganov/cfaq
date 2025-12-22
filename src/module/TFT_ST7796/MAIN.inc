#include "h.hpp"

// для спокойствия VScode
// #include <LovyanGFX.hpp>
#include "LGFX_dyn.hpp"


// ST7796_Config* ST7796_cfg = nullptr;
ST7796_Config ST7796_cfg;

LGFX tft;

uint16_t last_tft_color = TFT_WHITE;

// static
const ColorMap COLORS[] = {
    {"white",   TFT_WHITE},
    {"black",   TFT_BLACK},
    {"red",     TFT_RED},
    {"green",   TFT_GREEN},
    {"blue",    TFT_BLUE},
    {"cyan",    TFT_CYAN},
    {"magenta", TFT_MAGENTA},
    {"yellow",  TFT_YELLOW},
    {"orange",  TFT_ORANGE},
    {"pink",    TFT_PINK},
    {"purple",  TFT_PURPLE},
    {"navy",    TFT_NAVY},
    {"maroon",  TFT_MAROON},
    {"olive",   TFT_OLIVE},
    {"silver",  TFT_SILVER},
    {"brown",   TFT_BROWN},

    {"grey",  TFT_SILVER},
    {"gray",  TFT_SILVER},
};


uint16_t get_tft_color0(String s, uint16_t n) {
    String c = PARG(s, n);
    if (c == "") return last_tft_color;
    c.toLowerCase();

    for (auto& e : COLORS) {
        if (c == e.name) return e.value;
    }

    // HEX типа "FFFF"
    if (c.length() == 4) return (uint16_t) strtol(c.c_str(), NULL, 16); // Преобразуем HEX в число
    return last_tft_color;
}

uint16_t get_tft_color(String s, uint16_t n) {
    last_tft_color = get_tft_color0(s,n);
    return last_tft_color;
}

#include <vector>
std::vector<TouchMap> touchMap;
bool touch_ready = false;

void touch_add(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
              const String& value,
              const String& index,
              std::function<void(String,uint16_t,uint16_t)> fn)
{
    // Serial.println("Touch add: "+index+"="+value+" at: "+String(x)+" "+String(y)+" "+String(w)+" "+String(h));
    touchMap.push_back({x, y, w, h, index, value, fn});
}

void touch_del(const String& idx) {
    Serial.println("Touch del: "+idx);
    touchMap.erase(
        std::remove_if(
            touchMap.begin(),
            touchMap.end(),
            [&](const TouchMap& b){ return b.index == idx; }
        ),
        touchMap.end()
    );
}

void touch_clean() {
    Serial.println("Touch clean");
    touchMap.clear();
}

void touch_moto_fn(const String& s, uint16_t x, uint16_t y) {
    Serial.println("Touch MOTO: "+s);
    MOTO(s,0);
}

String keyboard_layout="";
void touch_key_fn(const String& s, uint16_t x, uint16_t y) {
    if(s == "fn") {
        if(keyboard_layout == "A") keyboard_layout = "a";
        else if(keyboard_layout == "a") keyboard_layout = "0";
        else if(keyboard_layout == "0") keyboard_layout = "!";
        else keyboard_layout = "A";
        touch_keyboard(keyboard_layout);
        return;
    }
    // // draw circle on key press
    // tft.fillCircle(x+15, y+15, 10, TFT_DARKGRAY);
    // delay(50);
    // tft.fillCircle(x+15, y+15, 10, TFT_WHITE);
    Serial.println("KEY: ["+s+"]");
}


// addTouch(10,10,50,40,"A","A", [](){ Serial.println("A pressed"); });
void touch_keyboard(String layout) {
    if (layout == "") layout=keyboard_layout;
    else keyboard_layout=layout;

    touch_del("KEY");

    int32_t xstart = 10;
    int32_t w = 30;
    int32_t h = 30;
    int32_t l = 4;
    int32_t x = xstart;
    int32_t y = 480-135;
    String az="-------------------------------------";

         if(layout == "A") az = "ABCDEFGHI" "JKLMNOPQR"   "STUVWXYZ.";
    else if(layout == "a") az = "abcdefghi" "jklmnopqr"   "stuvwxyz.";
    else if(layout == "0") az = "123456789" "0@+=-_$#^"   "(){}[]<>|";
    else if(layout == "!") az = ";:,.?!%&*" "\"\\'/     " "         ";   

    tft.fillRect(0, y-2, 320, 135+2, TFT_SILVER);


    for(uint16_t p = 0; p < az.length(); p++) {
        char c = az[p];
        tft.fillRoundRect(x, y, w, h, 8, TFT_WHITE);
        touch_add(x, y, w, h, "KEY", String(c), touch_key_fn);
        tft.setCursor( x+7, y+4 );
        tft.print( String(c) );
        x += w + l;
        if (p%9 == 8) {
            y += h + l;
            x = xstart;
        }
    }

    // shift
    tft.fillRoundRect(x, y, w*2, h, 8, TFT_DARKGRAY);
    tft.fillRoundRect(x, y, w*2, h, 8, TFT_DARKGRAY);
    touch_add(x, y, w*2, h, "KEY", "fn", touch_key_fn);
    tft.setCursor( x+7*3, y+4 );
    tft.print( "a" );
    x += (w + l)*3;

    // space
    tft.fillRoundRect(x, y, w*3, h, 8, TFT_WHITE);
    touch_add(x, y, w*3, h, "KEY", " ", touch_key_fn);
    x += (w + l)*4;

    // back
    tft.fillRoundRect(x, y, w*2, h, 8, TFT_DARKGRAY);
    touch_add(x, y, w*2, h, "KEY", "del", touch_key_fn);
    tft.setCursor( x+7*3, y+4 );
    tft.print( "<-" );
}
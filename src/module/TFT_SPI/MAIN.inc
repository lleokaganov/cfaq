//---------------- TFT SPI

#include "module/TFT_SPI/h.hpp"

// Создаём указатель на дисплей
Adafruit_ST7789* tft = nullptr;

uint16_t last_tft_color = ST77XX_WHITE;

uint16_t get_tft_color0(String s, uint16_t n) {
    String c = PARG(s,n);
    if(c == "") return last_tft_color;
    if(c == "white")   return ST77XX_WHITE;
    if(c == "black")   return ST77XX_BLACK;
    if(c == "red")     return ST77XX_RED;
    if(c == "green")   return ST77XX_GREEN;
    if(c == "blue")    return ST77XX_BLUE;
    if(c == "cyan")    return ST77XX_CYAN;
    if(c == "magenta") return ST77XX_MAGENTA;
    if(c == "yellow")  return ST77XX_YELLOW;
    if(c == "orange")  return ST77XX_ORANGE;
    // Если строка - это HEX-код (4 символа), пробуем конвертировать
    if(c.length() == 4) return strtol(c.c_str(), NULL, 16); // Преобразуем HEX в число
    return last_tft_color; // Если цвет не найден, возвращаем белый по умолчанию
}

uint16_t get_tft_color(String s, uint16_t n) {
    last_tft_color = get_tft_color0(s,n);
    return last_tft_color;
}

// #include <SPI.h>

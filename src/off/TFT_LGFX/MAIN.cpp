#include <LovyanGFX.hpp>
#include <TJpg_Decoder.h>

// TJpg_Decoder TJpgDec;

// ---------- DISPLAY SETUP ----------
class LGFX : public lgfx::LGFX_Device {
  lgfx::DisplayDriver _panel; // Panel_GC9A01 _panel;
  lgfx::Bus_SPI _bus;

public:
  LGFX() {
    _panel.setBus(&_bus);
    setPanel(&_panel);
  }

  // Добавляем доступ к реальному объекту шины и панели
  lgfx::Bus_SPI* getBusObject() { return &_bus; }
  lgfx::DisplayDriver* getPanelObject() { return &_panel; }

  void configure(uint8_t sclk, uint8_t mosi, uint8_t dc, uint8_t cs, uint8_t rst) {
    auto bus_cfg = _bus.config();
    bus_cfg.spi_host = SPI2_HOST;
    bus_cfg.spi_mode = 0;
    bus_cfg.freq_write = 40000000;
    bus_cfg.pin_sclk = sclk;
    bus_cfg.pin_mosi = mosi;
    bus_cfg.pin_miso = -1;
    bus_cfg.pin_dc   = dc;
    _bus.config(bus_cfg);
    auto panel_cfg = _panel.config();
    panel_cfg.pin_cs   = cs;
    panel_cfg.pin_rst  = rst;
    panel_cfg.pin_busy = -1;
    _panel.config(panel_cfg);
  }
};

LGFX tft;

uint16_t last_tft_color = TFT_WHITE;

uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

uint16_t get_tft_color0(String s, uint16_t n) {
    String c = PARG(s,n);
    c.toLowerCase();
    if(s.indexOf(",") > 0) return rgb565( PARG0(c,0,",",0), PARG0(c,1,",",0), PARG0(c,2,",",0) ); // 128,128,128
    if(c == "") return last_tft_color;
    if(c == "white")   return TFT_WHITE;
    if(c == "black")   return TFT_BLACK;
    if(c == "red")     return TFT_RED;
    if(c == "green")   return TFT_GREEN;
    if(c == "blue")    return TFT_BLUE;
    if(c == "cyan")    return TFT_CYAN;
    if(c == "magenta") return TFT_MAGENTA;
    if(c == "yellow")  return TFT_YELLOW;
    if(c == "orange")  return TFT_ORANGE;
    // HEX: "F00" (12-bit) или "F00F" (16-bit)
    if(c.length() == 4 || c.length() == 6) return strtol(c.c_str(), NULL, 16);
    return last_tft_color;
}

uint16_t get_tft_color(String s, uint16_t n) {
    last_tft_color = get_tft_color0(s,n);
    return last_tft_color;
}

bool jpegRender(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
  tft.pushImage(x, y, w, h, bitmap);
  return true;
}
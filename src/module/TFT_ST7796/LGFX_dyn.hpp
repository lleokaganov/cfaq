#pragma once

// чисто чтоб VScode не нервничал
#include <LovyanGFX.hpp>
#include <driver/spi_common.h>

struct ST7796_Config {
    // Включение модулей
    bool enable_display = true;
    bool enable_touch   = true;

    // --- BUS (ST7796) ---
    spi_host_device_t spi_host = VSPI_HOST;
    int spi_mode      = 0;
    int freq_write    = 40000000;  // твой текущий режим 40 MHz
    int freq_read     = 20000000;
    bool spi_3wire    = false;
    bool use_lock     = true;
    int dma_channel   = 1;

    int disp_sclk     = 18;
    int disp_mosi     = 23;
    int disp_miso     = -1;
    int disp_dc       = 2;

    // --- Panel pins ---
    int disp_cs       = 15;
    int disp_rst      = 4;
    int disp_busy     = -1;

    int memory_width  = 320;
    int memory_height = 480;
    int panel_width   = 320;
    int panel_height  = 480;

    // --- TOUCH (XPT2046) ---
    spi_host_device_t touch_spi_host = VSPI_HOST;
    int touch_freq     = 1000000; // 1 MHz

    int touch_sclk     = 18;
    int touch_mosi     = 23;
    int touch_miso     = 19;
    int touch_cs       = 5;

    int x_min = 200;
    int x_max = 3800;
    int y_min = 3800;
    int y_max = 200;

    bool bus_shared = false;
};



class LGFX : public lgfx::LGFX_Device {
public:
    lgfx::Panel_ST7796* panel = nullptr;
    lgfx::Bus_SPI* bus = nullptr;
    lgfx::Touch_XPT2046* touch = nullptr;

    LGFX() {}

    void initFromConfig(const ST7796_Config& cfg) {

        // --- DISPLAY ---
        if (cfg.enable_display) {
            bus = new lgfx::Bus_SPI();
            panel = new lgfx::Panel_ST7796();

            // -------- BUS CONFIG ----------
            auto b = bus->config();
            b.spi_host    = cfg.spi_host;
            b.spi_mode    = cfg.spi_mode;
            b.freq_write  = cfg.freq_write;
            b.freq_read   = cfg.freq_read;
            b.spi_3wire   = cfg.spi_3wire;
            b.use_lock    = cfg.use_lock;
            b.dma_channel = cfg.dma_channel;

            b.pin_sclk = cfg.disp_sclk;
            b.pin_mosi = cfg.disp_mosi;
            b.pin_miso = cfg.disp_miso;
            b.pin_dc   = cfg.disp_dc;

            bus->config(b);
            panel->setBus(bus);

            // -------- PANEL CONFIG ----------
            auto p = panel->config();
            p.pin_cs   = cfg.disp_cs;
            p.pin_rst  = cfg.disp_rst;
            p.pin_busy = cfg.disp_busy;

            p.memory_width  = cfg.memory_width;
            p.memory_height = cfg.memory_height;
            p.panel_width   = cfg.panel_width;
            p.panel_height  = cfg.panel_height;

            panel->config(p);
            setPanel(panel);
        }

        // --- TOUCH (опционально) ---
        if (cfg.enable_touch) {
            touch = new lgfx::Touch_XPT2046();

            auto t = touch->config();
            t.spi_host = cfg.touch_spi_host;
            t.freq     = cfg.touch_freq;

            t.pin_sclk = cfg.touch_sclk;
            t.pin_mosi = cfg.touch_mosi;
            t.pin_miso = cfg.touch_miso;
            t.pin_cs   = cfg.touch_cs;

            t.x_min = cfg.x_min;
            t.x_max = cfg.x_max;
            t.y_min = cfg.y_min;
            t.y_max = cfg.y_max;

            t.bus_shared = cfg.bus_shared;

            touch->config(t);

            if (panel) panel->setTouch(touch);
        }

        // Запускаем дисплей ровно один раз
        if (panel) init();
    }

    // void initFromConfig(const ST7796_Config& cfg) {

    //     // --- DISPLAY ---
    //     if (cfg.enable_display) {
    //         bus = new lgfx::Bus_SPI();
    //         panel = new lgfx::Panel_ST7796();

    //         auto b = bus->config();
    //         b.pin_sclk = cfg.disp_sclk;
    //         b.pin_mosi = cfg.disp_mosi;
    //         b.pin_miso = cfg.disp_miso;
    //         b.pin_dc   = cfg.disp_dc;
    //         b.freq_write = cfg.freq_write;
    //         b.freq_read  = cfg.freq_read;
    //         bus->config(b);

    //         auto p = panel->config();
    //         p.pin_cs  = cfg.disp_cs;
    //         p.pin_rst = cfg.disp_rst;
    //         panel->config(p);

    //         panel->setBus(bus);
    //         setPanel(panel);
    //     }

    //     // --- TOUCH (опционально) ---
    //     if (cfg.enable_touch) {
    //         touch = new lgfx::Touch_XPT2046();

    //         auto t = touch->config();
    //         t.pin_sclk = cfg.touch_sclk;
    //         t.pin_mosi = cfg.touch_mosi;
    //         t.pin_miso = cfg.touch_miso;
    //         t.pin_cs   = cfg.touch_cs;
    //         t.freq     = cfg.touch_freq;
    //         t.x_min = cfg.x_min;
    //         t.x_max = cfg.x_max;
    //         t.y_min = cfg.y_min;
    //         t.y_max = cfg.y_max;
    //         touch->config(t);

    //         // Только если есть панель
    //         if (panel) panel->setTouch(touch);
    //     }

    //     // Стартуем библиотеку ровно один раз
    //     if (panel) init();
    // }
};
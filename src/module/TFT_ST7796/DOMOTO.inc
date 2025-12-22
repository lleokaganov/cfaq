// #include "h.hpp"

if (cmd =="touch.set.calibrate") { // { 300, 3200, 3500, 3200,  300,  500, 3500, 500 };
    uint16_t tp[8] = {0};
    for(int i = 0; i < 8; i++) tp[i] = PARG0(s, i + 1);
    tft.setTouchCalibrate(tp);
    return EMPTY_STRING;
}

if (cmd == "touch.load.calibrate") {
    String tps = getfile("/touch_calibrate.txt");
    if(tps == "") cmd = "touch.save.calibrate";
    else {
        uint16_t tp[8] = {0};
        for(int i = 0; i < 8; i++) tp[i] = PARG0(tps, i);
        for(int i = 0; i < 8; i++) {
            Serial.print(String(tp[i]));
            if(i<7) Serial.print(" ");
        }
        tft.setTouchCalibrate(tp);
        return EMPTY_STRING;
    }
}

if (cmd == "touch.save.calibrate") {
    uint16_t tp[8] = {0};    
    tft.calibrateTouch(tp, TFT_WHITE, TFT_BLACK, 40);
    Serial.print("\nTouch calibrated1: ");
    String tps = "";
    for(int i = 0; i < 8; i++) {
        Serial.print(String(tp[i]));
        tps+=String(tp[i]);
        if(i<7) {
            Serial.print(" ");
            tps+=" ";
        }
    }
    Serial.println("\nTouch calibrated2: " + tps);
    file_save("/touch_calibrate.txt", tps);
    return EMPTY_STRING;
}

if (cmd == "touch.clean") {
    touch_clean();
    return EMPTY_STRING;
}

if (cmd == "touch.add") {
    touch_add(
        PARG0(s, 1), // x
        PARG0(s, 2), // y
        PARG0(s, 3), // w
        PARG0(s, 4), // h
        PARG(s, 5), // index
        PARG_OTHER(s, 5), // value
        touch_moto_fn
    );
    return EMPTY_STRING;
}

if (cmd =="touch.del") {
    touch_del( PARG(s, 1)); // index
    return EMPTY_STRING;
}

// ====== keyboard ======


if (cmd == "tft.KEYBOARD") {
    touch_keyboard("A");
    return EMPTY_STRING;
}


/* Init script example:


tft.set.enable_touch true
tft.set.spi_mode 0
tft.set.freq_read 20000000
tft.set.spi_3wire false
tft.set.use_lock true
tft.set.dma_channel 1

tft.set.disp_sclk 18
tft.set.disp_mosi 23
tft.set.disp_miso -1
tft.set.disp_dc 2

tft.set.disp_cs 15
tft.set.disp_rst 4
tft.set.disp_busy -1

tft.set.memory_width 320
tft.set.memory_height 480
tft.set.panel_width 320
tft.set.panel_height 480

tft.set.touch_freq 1000000

tft.set.touch_sclk 18
tft.set.touch_mosi 23
tft.set.touch_miso 19
tft.set.touch_cs 5

tft.set.x_min 200
tft.set.x_max 3800
tft.set.y_min 3800
tft.set.y_max 200

tft.set.bus_shared false
*/

















if(CMD_EQ("tft.rotation")) {
    tft.setRotation(PARG0(s,1));
    return EMPTY_STRING;
}

if(CMD_EQ("tft.clear")) { // Очистка экрана цветом
    Serial.printf("CLEAR BEGIN tft at %p\n", &tft);

    tft.fillScreen( get_tft_color(s,1) );
    return EMPTY_STRING;
}


// ==== image from file ====

if(CMD_EQ("tft.jpg")) {
    String name = Slash(PARG_OTHER(s,0));
    File f = SPIFFS.open(name, "r");
    if (!f) return ERR(F("File not found"), name);
    bool ok = tft.drawJpg(&f, 0, 0);
    f.close();
    if (!ok) ERR(F("JPEG error: convert 1.jpg -interlace none 2.jpg"));
    return EMPTY_STRING;
}

if(CMD_EQ("tft.png")) {
    String name = Slash(PARG_OTHER(s,0));
    File f = SPIFFS.open(name, "r");
    if (!f) return ERR(F("File not found"), name);
    bool ok = tft.drawPng(&f, 0, 0);
    f.close();
    if (!ok) ERR(F("PNG error"));
    return EMPTY_STRING;
}

if (cmd == "tft.raw") {   // tft.raw x y file_X_Y.raw
    String name = Slash(PARG_OTHER(s, 2));
    File file = SPIFFS.open(name, "r");
    if (!file) return ERR(F("File not found"), name);

    // имя вида "file_320_240.raw"
    uint16_t w = PARG0(name, 1, "_");
    uint16_t h = PARG0(PARG(name, 2, "_"), 0, ".");
    uint16_t x = PARG0(s, 1);
    uint16_t y = PARG0(s, 2);

    uint16_t *buffer = new uint16_t[w];
    if (!buffer) { file.close(); return ERR(F("ERROR: no RAM for RAW buffer")); }

    for (uint16_t row = 0; row < h; row++) {
        int got = file.read((uint8_t *)buffer, w * 2);
        if (got != w * 2) break;  // файл короче ожидаемого
        tft.pushImage(x, y + row, w, 1, buffer);
    }

    delete[] buffer;
    file.close();
    return EMPTY_STRING;
}


// ==== init and settings ====

if (cmd == "tft.begin") {
    Serial.printf("BEGIN 1 tft at %p\n", &tft);
    tft.initFromConfig(ST7796_cfg);
    Serial.printf("BEGIN 2 tft at %p\n", &tft);
    return EMPTY_STRING;
}

if(CMD_EQ("tft.set.enable_display")) { ST7796_cfg.enable_display = PARGB(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.enable_touch")) { ST7796_cfg.enable_touch = PARGB(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.freq_write")) { ST7796_cfg.freq_write = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.spi_mode")) { ST7796_cfg.spi_mode = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.freq_read")) { ST7796_cfg.freq_read = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.spi_3wire")) { ST7796_cfg.spi_3wire = PARGB(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.use_lock")) { ST7796_cfg.use_lock = PARGB(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.dma_channel")) { ST7796_cfg.dma_channel = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.disp_sclk")) { ST7796_cfg.disp_sclk = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.disp_mosi")) { ST7796_cfg.disp_mosi = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.disp_miso")) { ST7796_cfg.disp_miso = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.disp_dc")) { ST7796_cfg.disp_dc = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.disp_cs")) { ST7796_cfg.disp_cs = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.disp_rst")) { ST7796_cfg.disp_rst = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.disp_busy")) { ST7796_cfg.disp_busy = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.memory_width")) { ST7796_cfg.memory_width = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.memory_height")) { ST7796_cfg.memory_height = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.panel_width")) { ST7796_cfg.panel_width = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.panel_height")) { ST7796_cfg.panel_height = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.touch_freq")) { ST7796_cfg.touch_freq = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.touch_sclk")) { ST7796_cfg.touch_sclk = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.touch_mosi")) { ST7796_cfg.touch_mosi = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.touch_miso")) { ST7796_cfg.touch_miso = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.touch_cs")) { ST7796_cfg.touch_cs = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.x_min")) { ST7796_cfg.x_min = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.x_max")) { ST7796_cfg.x_max = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.y_min")) { ST7796_cfg.y_min = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.y_max")) { ST7796_cfg.y_max = PARG0(s,1); return EMPTY_STRING; }
if(CMD_EQ("tft.set.bus_shared")) { ST7796_cfg.bus_shared = PARGB(s,1); return EMPTY_STRING; }

//  Т Е К С Т

// void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
if(cmd == "tft.char") {  // Выводим символ x, y,CHAR, color, bg, size
    tft.drawChar(PARG0(s,1), PARG0(s,2), (char)PARG(s,3)[0], get_tft_color(s,4), get_tft_color(s,5), PARG0(s,6));
    return EMPTY_STRING;
}

// void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
if(cmd == "tft.charxy") {  // Выводим символ с разными размерами по X и Y
    tft.drawChar(PARG0(s,1), PARG0(s,2), (char)PARG(s,3)[0], get_tft_color(s,4), get_tft_color(s,5), PARG0(s,6), PARG0(s,7));
    return EMPTY_STRING;
}

if(CMD_EQ("tft.font")) { // Размер шрифта [или X Y]
    uint8_t sy = PARG0(s,2);
    if (sy) tft.setTextSize(PARG0(s,1), sy);
    else    tft.setTextSize(PARG0(s,1));
    return EMPTY_STRING;
}

if(CMD_EQ("tft.color")) { // Цвет шрифта [и фона]
    if (PARG(s,2) == "") tft.setTextColor(get_tft_color(s,1));
    else tft.setTextColor(get_tft_color(s,1), get_tft_color(s,2));
    return EMPTY_STRING;
}

if(CMD_EQ("tft.cursor")) { // Координаты курсора
    tft.setCursor( PARG0(s,1), PARG0(s,2) );
    return EMPTY_STRING;
}

if(CMD_EQ("tft.print")) { // Цвет шрифта
    String txt = PARG_OTHER(s,0);
    txt.replace("\\n","\n");
    tft.print( txt );
    return EMPTY_STRING;
}

if(cmd == "tft.text") {  // Выводим текст по координатам
    tft.setCursor( PARG0(s,1), PARG0(s,2) );
    String txt = PARG_OTHER(s,2);
    txt.replace("\\n","\n");
    tft.print( txt );
    return EMPTY_STRING;
}

// Т О Ч К И    И    Л И Н И И

if(cmd == "tft.pixel") {  // Рисуем пиксель
    tft.drawPixel(PARG0(s,1), PARG0(s,2), get_tft_color(s, 3));
    return EMPTY_STRING;
}

if(cmd == "tft.writepixel") {  // Записываем пиксель
    tft.writePixel(PARG0(s,1), PARG0(s,2), get_tft_color(s,3));
    return EMPTY_STRING;
}

if(cmd == "tft.line") {  // Рисуем линию
    tft.drawLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return EMPTY_STRING;
}

if(cmd == "tft.fastvline") {  // Быстрая вертикальная линия
    tft.drawFastVLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.fasthline") {  // Быстрая горизонтальная линия
    tft.drawFastHLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.writefastvline") {  // Быстрая вертикальная линия (запись)
    tft.writeFastVLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.writefasthline") {  // Быстрая горизонтальная линия (запись)
    tft.writeFastHLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.writeline") {  // Линия (оптимизированная запись)
    tft.drawLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return EMPTY_STRING;
}

// Ф И Г У Р Ы


if(cmd == "tft.rect") {  // Рисуем прямоугольник
    tft.drawRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s, 5));
    return EMPTY_STRING;
}

if(cmd == "tft.fillrect") {  // Заполняем прямоугольник
    uint16_t color = get_tft_color(s, 5);
    tft.fillRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s, 5));
    return EMPTY_STRING;
}

if(cmd == "tft.writefillrect") {  // Быстро заполняем прямоугольник
    tft.writeFillRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return EMPTY_STRING;
}

if(cmd == "tft.circle") {  // Рисуем круг
    tft.drawCircle(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.fillcircle") {  // Заполняем круг
    tft.fillCircle(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.triangle") {  // Рисуем треугольник
    tft.drawTriangle(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), PARG0(s,6), get_tft_color(s,7));
    return EMPTY_STRING;
}

if(cmd == "tft.filltriangle") {  // Заполняем треугольник
    tft.fillTriangle(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), PARG0(s,6), get_tft_color(s,7));
    return EMPTY_STRING;
}

if(cmd == "tft.circlehelper") {  // Круговой помощник
    tft.drawCircleHelper(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return EMPTY_STRING;
}

if(cmd == "tft.fillcirclehelper") {  // Заполняемый круговой помощник
    tft.fillCircleHelper(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), get_tft_color(s,6));
    return EMPTY_STRING;
}

if(cmd == "tft.roundrect") {  // Рисуем скругленный прямоугольник
    tft.drawRoundRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), get_tft_color(s,6));
    return EMPTY_STRING;
}

if(cmd == "tft.fillroundrect") {  // Заполняем скругленный прямоугольник
    tft.fillRoundRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), get_tft_color(s,6));
    return EMPTY_STRING;
}


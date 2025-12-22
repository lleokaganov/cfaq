/*

SPI.begin 36 -1 35
tft.begin -1 37 38
tft.init 240 280 3
tft.rotation 2
tft.clear black
tft.font 2
tft.color white black
tft.cursor 15 15
tft.raw 0 0 /t_240_280.raw
tft.print Tasha


SPI.begin 36 -1 35
tft.begin -1 37 38
tft.rotation 1
tft.clear blue
tft.font 6
tft.color green
tft.print WoWOWOW

....
    // Выводим текст
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(20, 40);
    tft.print("Hello, ESP32-S3!");
....


// Определяем пины
  #define TFT_RST    38  // Reset (RES) RES  → Reset (RST)
  #define TFT_DC     37  // Data/Command (DC) DC   → Data/Command (DC)
  #define TFT_SCK    36  // Clock (SCK) SCL  → Clock (SCK)
  #define TFT_MOSI   35  // MOSI (DATA) SDA  → MOSI (DATA)
// CS   → Chip Select (CS)..
// BLK  → Подсветка (Backlight)..

// Создаём объект дисплея **без CS**
Adafruit_ST7789 tft = Adafruit_ST7789(-1, TFT_DC, TFT_RST);  // -1 означает, что CS не используется

    // Инициализация SPI без CS
    SPI.begin(TFT_SCK, -1, TFT_MOSI);  // (SCK, MISO, MOSI)
*/

if(CMD_EQ("tft.sendCommand")) {
    // Найдем команду
    String arg = ARG(s,1); if(arg == "") { ERR("Err command"); return EMPTY_STRING; }
    uint8_t command = (uint8_t)strtoul(arg.c_str(), NULL, 16);
    int argCount = 0; while ( ARG(s,argCount+2) != "") argCount++; // Подсчет аргументов
    if(argCount == 0) { ERR("No args"); return EMPTY_STRING; }
    // Выделяем память под аргументы
    uint8_t* data = (uint8_t*)malloc(argCount * sizeof(uint8_t));
    if(data == NULL) { ERR("No memory"); return EMPTY_STRING; }

    // Заполняем массив числами из строки
    for(int i=0; i<argCount; i++) {
        String arg = ARG(s, i+2);
        data[i] = (uint8_t)strtoul(arg.c_str(), NULL, 16);
    }

//    ERR("Command: "+HH0X(command)); // Преобразование))
//    for (int i = 0; i < argCount; i++) {
//	ERR("Data #"+String(i+1)+": "+HH0X(data[i])); // Преобразование))
//    }
    tft->sendCommand(command, data, sizeof(data));
    return EMPTY_STRING;
}


if(CMD_EQ("tft.raw")) { // Вывести картинку RAW tft.raw x y file_X_Y.raw
    String name = Slash( PARG_OTHER(s,2) );
    if(!is_file(name)) { ERR(String(F("ERROR: File not found ["))+String(name)+String(F("]"))); return EMPTY_STRING; }
    File file = SPIFFS.open( name , "r");
    uint16_t w = PARG0(name,1,"_");
    uint16_t h = PARG0(PARG(name,2,"_"),0,".");
    uint16_t x = PARG0(s,1);
    uint16_t y = PARG0(s,2);

    LOGI(LOG_PROG, String("File: ")+String(name));
    LOGI(LOG_PROG, String("w: ")+String(w));
    LOGI(LOG_PROG, String("h: ")+String(h));
    LOGI(LOG_PROG, String("x: ")+String(x));
    LOGI(LOG_PROG, String("y: ")+String(y));

    uint16_t *buffer = new uint16_t[w];
    for(int16_t row=0; row<h; row++) {
        file.read((uint8_t*)buffer, w*2);
        tft->drawRGBBitmap(x, y+row, buffer, w, 1);
    }
    delete[] buffer;
    file.close();
    return EMPTY_STRING;
}


if(CMD_EQ("tft.begin")) { // Создать дисплей tft->begin -1 {DC_PIN} {RST_PIN}
    int8_t cs = PARG0(s,1," ",-1);
    int8_t dc = PARG0(s,2," ",-1);
    int8_t rst = PARG0(s,3," ",-1);
    int8_t mosi = PARG0(s,4," ",-1);
    int8_t sclk = PARG0(s,5," ",-1);
    #ifdef ESP8266
	// tft = new Adafruit_ST7789(SPIClass *spiClass, cs, dc, rst);
    #else
        if(mosi<0 && sclk<0) tft = new Adafruit_ST7789(cs, dc, rst);
	else tft = new Adafruit_ST7789(cs, dc, mosi, sclk, rst);
    #endif
    return EMPTY_STRING;
}

if(CMD_EQ("tft.init")) { // Инициализация дисплея tft->init 240 280 3
    uint16_t W = PARG0(s,1," ",240);
    uint16_t H = PARG0(s,2," ",280);
    uint8_t SPI_MODE = PARG0(s,3);
    tft->init(W, H, SPI_MODE); // Иногда ST7789 работает в режиме SPI_MODE3
    return EMPTY_STRING;
}


if(CMD_EQ("tft.rotation")) { // Поворот экрана (0-3)
    uint8_t R = PARG0(s,1);
    tft->setRotation(R);
    return EMPTY_STRING;
}

if(CMD_EQ("tft.clear")) { // Очистка экрана цветом
    tft->fillScreen( get_tft_color(s,1) );
    return EMPTY_STRING;
}

if(cmd == "tft.invert") {  // Инверсия дисплея
    tft->invertDisplay( PARG0(s,1) );
    return EMPTY_STRING;
}




//  Т Е К С Т


if(CMD_EQ("tft.font")) { // Размер шрифта [или X Y]
    uint8_t sy = PARG0(s,2);
    if(sy) tft->setTextSize( PARG0(s,1), sy);
    else tft->setTextSize(PARG0(s,1));
    return EMPTY_STRING;
}

if(CMD_EQ("tft.color")) { // Цвет шрифта [и фона]
    if( PARG(s,2) == "" ) tft->setTextColor( get_tft_color(s,1) );
    else tft->setTextColor( get_tft_color(s,1), get_tft_color(s,2) );
    return EMPTY_STRING;
}

if(CMD_EQ("tft.cursor")) { // Координаты курсора
    tft->setCursor( PARG0(s,1), PARG0(s,2) );
    return EMPTY_STRING;
}

if(CMD_EQ("tft.print")) { // Цвет шрифта
    String txt = PARG_OTHER(s,0);
    txt.replace("\\n","\n");
    tft->print( txt );
    return EMPTY_STRING;
}

if(cmd == "tft.text") {  // Выводим текст по координатам
    tft->setCursor( PARG0(s,1), PARG0(s,2) );
    String txt = PARG_OTHER(s,2);
    txt.replace("\\n","\n");
    tft->print( txt );
    return EMPTY_STRING;
}



// Т О Ч К И    И    Л И Н И И

if(cmd == "tft.pixel") {  // Рисуем пиксель
    tft->drawPixel(PARG0(s,1), PARG0(s,2), get_tft_color(s, 3));
    return EMPTY_STRING;
}

if(cmd == "tft.writepixel") {  // Записываем пиксель
    tft->writePixel(PARG0(s,1), PARG0(s,2), get_tft_color(s,3));
    return EMPTY_STRING;
}

if(cmd == "tft.line") {  // Рисуем линию
    tft->drawLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return EMPTY_STRING;
}

if(cmd == "tft.fastvline") {  // Быстрая вертикальная линия
    tft->drawFastVLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.fasthline") {  // Быстрая горизонтальная линия
    tft->drawFastHLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.writefastvline") {  // Быстрая вертикальная линия (запись)
    tft->writeFastVLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.writefasthline") {  // Быстрая горизонтальная линия (запись)
    tft->writeFastHLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.writeline") {  // Линия (оптимизированная запись)
    tft->writeLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return EMPTY_STRING;
}



// Ф И Г У Р Ы


if(cmd == "tft.rect") {  // Рисуем прямоугольник
    tft->drawRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s, 5));
    return EMPTY_STRING;
}

if(cmd == "tft.fillrect") {  // Заполняем прямоугольник
    uint16_t color = get_tft_color(s, 5);
    tft->fillRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s, 5));
    return EMPTY_STRING;
}

if(cmd == "tft.writefillrect") {  // Быстро заполняем прямоугольник
    tft->writeFillRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return EMPTY_STRING;
}

if(cmd == "tft.circle") {  // Рисуем круг
    tft->drawCircle(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.fillcircle") {  // Заполняем круг
    tft->fillCircle(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return EMPTY_STRING;
}

if(cmd == "tft.triangle") {  // Рисуем треугольник
    tft->drawTriangle(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), PARG0(s,6), get_tft_color(s,7));
    return EMPTY_STRING;
}

if(cmd == "tft.filltriangle") {  // Заполняем треугольник
    tft->fillTriangle(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), PARG0(s,6), get_tft_color(s,7));
    return EMPTY_STRING;
}

if(cmd == "tft.circlehelper") {  // Круговой помощник
    tft->drawCircleHelper(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return EMPTY_STRING;
}

if(cmd == "tft.fillcirclehelper") {  // Заполняемый круговой помощник
    tft->fillCircleHelper(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), get_tft_color(s,6));
    return EMPTY_STRING;
}

if(cmd == "tft.roundrect") {  // Рисуем скругленный прямоугольник
    tft->drawRoundRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), get_tft_color(s,6));
    return EMPTY_STRING;
}

if(cmd == "tft.fillroundrect") {  // Заполняем скругленный прямоугольник
    tft->fillRoundRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), get_tft_color(s,6));
    return EMPTY_STRING;
}



/*
if(cmd == "tft.startwrite") {  // Начинаем запись
    tft->startWrite();
    return EMPTY_STRING;
}

if(cmd == "tft.endwrite") {  // Завершаем запись
    tft->endWrite();
    return EMPTY_STRING;
}
*/


    // void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
    if(cmd == "tft.char") {  // Выводим символ x, y,CHAR, color, bg, size
        tft->drawChar(PARG0(s,1), PARG0(s,2), (char)PARG(s,3)[0], get_tft_color(s,4), get_tft_color(s,5), PARG0(s,6));
        return EMPTY_STRING;
    }

    // void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
    if(cmd == "tft.charxy") {  // Выводим символ с разными размерами по X и Y
        tft->drawChar(PARG0(s,1), PARG0(s,2), (char)PARG(s,3)[0], get_tft_color(s,4), get_tft_color(s,5), PARG0(s,6), PARG0(s,7));
        return EMPTY_STRING;
    }


/*
 void drawPixel(int16_t x, int16_t y, uint16_t color);
 void writePixel(int16_t x, int16_t y, uint16_t color);

 void startWrite(void);
 void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
 void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
 void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
 void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
 void endWrite(void);

  void byteSwap(void);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

  void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
  void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
  void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
  void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);

  void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
  void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);



  int16_t width(void) const { return _width; };
  int16_t height(void) const { return _height; }
  uint8_t getRotation(void) const { return rotation; }
  int16_t getCursorX(void) const { return cursor_x; }
  int16_t getCursorY(void) const { return cursor_y; };
  void getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
  void getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
  void getTextBounds(const String &str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);



  void setFont(const GFXfont *f = NULL);

  void setTextWrap(bool w) { wrap = w; }

  void initButton(Adafruit_GFX *gfx, int16_t x, int16_t y, uint16_t w,
                  uint16_t h, uint16_t outline, uint16_t fill,
                  uint16_t textcolor, char *label, uint8_t textsize);
  void initButton(Adafruit_GFX *gfx, int16_t x, int16_t y, uint16_t w,
                  uint16_t h, uint16_t outline, uint16_t fill,
                  uint16_t textcolor, char *label, uint8_t textsize_x,
                  uint8_t textsize_y);
  // New/alt initButton() uses upper-left corner & size
  void initButtonUL(Adafruit_GFX *gfx, int16_t x1, int16_t y1, uint16_t w,
                    uint16_t h, uint16_t outline, uint16_t fill,
                    uint16_t textcolor, char *label, uint8_t textsize);
  void initButtonUL(Adafruit_GFX *gfx, int16_t x1, int16_t y1, uint16_t w,
                    uint16_t h, uint16_t outline, uint16_t fill,
                    uint16_t textcolor, char *label, uint8_t textsize_x,
                    uint8_t textsize_y);
  void drawButton(bool inverted = false);
  bool contains(int16_t x, int16_t y);


  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg);
  void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
  void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
  void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
  void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
  void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
  void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
  void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
  void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
  void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
  void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
  void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h);

*/
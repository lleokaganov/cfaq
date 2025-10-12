/*

TFT.begin sclk=39 mosi=40 dc=2 cs=1 rst=3
tft.init 240 240
tft.rotation 0
tft.invert 1
tft.clear black

camera2tft.start 33

*/
if(cmd=="TFT.begin") { // запустить камеру
    if(s.indexOf("=") < 0) tft.configure(
        PARG0(s,5," ",-1), // sclk,
        PARG0(s,4," ",-1), // mosi,
        PARG0(s,2," ",-1), // dc,
        PARG0(s,1," ",-1), // cs,
        PARG0(s,3," ",-1)  // rst
    ); else tft.configure(
	FARG0(s, "sclk", 39), // sclk,
	FARG0(s, "mosi", 40), // mosi,
	FARG0(s, "dc",   2),  // dc,
	FARG0(s, "cs",   1),  // cs,
	FARG0(s, "rst",  3)   // rst
    );
//    tft.init();
//--> tft.rotation 0   tft.setRotation(FARG0(s,"rotation", 0));
//--> tft.invert 1    tft.invertDisplay(FARG0(s,"invert", 1)); // Включить инверсию
//--> tft.clear BLACK   tft.fillScreen(TFT_BLACK);

    TJpgDec.setSwapBytes(true);
    TJpgDec.setJpgScale(1);
    TJpgDec.setCallback(jpegRender);

    delay(200);
    return "";
}

if(cmd == "tft.init") {
    uint16_t W = PARG0(s,1," ",240);
    uint16_t H = PARG0(s,2," ",280);
    uint8_t SPI_MODE = PARG0(s,3," ",0);  // SPI_MODE по умолчанию = 0

    // auto bus_cfg = tft.getBus()->config();
    auto bus_cfg = tft.getBusObject()->config();
    bus_cfg.spi_mode = SPI_MODE; // Иногда ST7789 работает в режиме SPI_MODE3
    // tft.getBus()->config(bus_cfg);
    tft.getBusObject()->config(bus_cfg);

    // auto panel_cfg = tft.getPanel()->config();
    auto panel_cfg = tft.getPanelObject()->config();
    panel_cfg.panel_width  = W;
    panel_cfg.panel_height = H;
    panel_cfg.memory_width  = W;
    panel_cfg.memory_height = H;
    // tft.getPanel()->config(panel_cfg);
    tft.getPanelObject()->config(panel_cfg);

    tft.init();
    return "";
}

/*
// ---------- JPEG CALLBACK ----------
bool jpegRender(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
  for(int i=0; i< w*h; i++) {
    uint16_t px = bitmap[i];
    bitmap[i] = (px << 8) | (px >> 8);  // MSB ↔ LSB
  }
  tft.pushImage(x, y, w, h, bitmap);
  return true;
}





  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(jpegRender);
  delay(200);

  if(!camera_init()) { Serial.println("❌ Camera error"); while(1); }
  Serial.println("✅ Camera OK");
  delay(200);

void loop() {
  cam_fb = esp_camera_fb_get();
  if(cam_fb) {
    TJpgDec.drawJpg(0, 0, cam_fb->buf, cam_fb->len);
    esp_camera_fb_return(cam_fb);
  } else {
    Serial.println("❌ Frame error");
  }
   delay(1);
}
*/

/*
if(cmd=="tft.sendCommand") {
    // Найдем команду
    String arg = ARG(s,1); if(arg == "") { ERR("Err command"); return ""; }
    uint8_t command = (uint8_t)strtoul(arg.c_str(), NULL, 16);
    int argCount = 0; while ( ARG(s,argCount+2) != "") argCount++; // Подсчет аргументов
    if(argCount == 0) { ERR("No args"); return ""; }
    // Выделяем память под аргументы
    uint8_t* data = (uint8_t*)malloc(argCount * sizeof(uint8_t));
    if(data == NULL) { ERR("No memory"); return ""; }

    // Заполняем массив числами из строки
    for(int i=0; i<argCount; i++) {
        String arg = ARG(s, i+2);
        data[i] = (uint8_t)strtoul(arg.c_str(), NULL, 16);
    }

//    ERR("Command: "+String(command, HEX)); // Преобразование))
//    for (int i = 0; i < argCount; i++) {
//	ERR("Data #"+String(i+1)+": "+String(data[i], HEX)); // Преобразование))
//    }
    // tft.sendCommand(command, data, sizeof(data));
    tft.getPanelObject()->sendCommand(command, data, sizeof(data));
    return "";
}
*/

if(cmd == "tft.sendCommand") {
    String arg = ARG(s,1); if(arg == "") { ERR("Err command"); return ""; }
    uint8_t command = (uint8_t)strtoul(arg.c_str(), NULL, 16);
    int argCount = 0; while (ARG(s, argCount+2) != "") argCount++;
    if(argCount == 0) { ERR("No args"); return ""; }
    // Выделяем память под аргументы
    uint8_t* data = (uint8_t*)malloc(argCount * sizeof(uint8_t));
    if(data == NULL) { ERR("No memory"); return ""; }
    for(int i=0; i<argCount; i++) data[i] = (uint8_t)strtoul(ARG(s,i+2).c_str(), NULL, 16);

    auto panel = tft.getPanelObject();
    panel->beginTransaction();

    panel->writeCommand(command,8); // 8 бит
    // panel->writeData(data, argCount);
    for(int i=0; i<argCount; i++) panel->writeData(data[i], 8);

    panel->endTransaction();

    free(data);
    return "";
}

/*
if(cmd == "tft.jpg") {
  String name = Slash(PARG_OTHER(s,2));
  if(!is_file(name)) { ERR("File not found: " + name); return ""; }
  uint16_t x = PARG0(s,1," ",0);
  uint16_t y = PARG0(s,2," ",0);
  File f = SPIFFS.open(name, "r");
  if(!f) { ERR("Open failed: " + name); return ""; }
    String ras = ARG(name,1,".");
    ras.toUpperCase();
    if(ras == "JPG" || ras == "JPEG") TJpgDec.drawJpg(f, x, y); // <-- если перегрузка доступна
//TJpgDec.drawJpg(&f, (int32_t)x, (int32_t)y);
	// TJpgDec.drawJpgFile(SPIFFS, name.c_str(), x, y);

    else if(ras == "BMP") tft.drawBmp(&f, x, y); // если используешь встроенный метод LovyanGFX
    // else if(ras == "PNG") png.open(f, PNGDrawCallback); // если подключена PNGdec
  f.close();
  return "";
}
*/

if(cmd=="tft.raw") { // Вывести картинку RAW tft.raw x y file_X_Y.raw
    String name = Slash( PARG_OTHER(s,2) );
    if(!is_file(name)) { ERR(String(F("ERROR: File not found ["))+String(name)+String(F("]"))); return ""; }
    File file = SPIFFS.open( name , "r");
    uint16_t w = PARG0(name,1,"_");
    uint16_t h = PARG0(PARG(name,2,"_"),0,".");
    uint16_t x = PARG0(s,1);
    uint16_t y = PARG0(s,2);

    LOGI(LOG_UPGRADE, String("File: ")+String(name));
    LOGI(LOG_UPGRADE, String("w: ")+String(w));
    LOGI(LOG_UPGRADE, String("h: ")+String(h));
    LOGI(LOG_UPGRADE, String("x: ")+String(x));
    LOGI(LOG_UPGRADE, String("y: ")+String(y));

    tft.startWrite();

    uint16_t *buffer = new uint16_t[w];
    for(int16_t row=0; row<h; row++) {
        file.read((uint8_t*)buffer, w*2);
        // tft.drawRGBBitmap(x, y+row, buffer, w, 1);
	tft.pushImage(x, y+row, w, 1, buffer);
    }
    delete[] buffer;
    file.close();

    tft.endWrite();

    return "";
}

if(cmd=="tft.rotation") { // Поворот экрана (0-3)
    tft.setRotation( PARG0(s,1) );
    return "";
}

if(cmd=="tft.clear") { // Очистка экрана цветом
    tft.fillScreen( get_tft_color(s,1) );
    return "";
}

if(cmd == "tft.invert") {  // Инверсия дисплея
    tft.invertDisplay( PARG0(s,1) );
    return "";
}


//  Т Е К С Т


if(cmd=="tft.font") { // Размер шрифта [или X Y]
    uint8_t sy = PARG0(s,2);
    if(sy) tft.setTextSize( PARG0(s,1), sy);
    else tft.setTextSize(PARG0(s,1));
    return "";
}

if(cmd=="tft.color") { // Цвет шрифта [и фона]
    if( PARG(s,2) == "" ) tft.setTextColor( get_tft_color(s,1) );
    else tft.setTextColor( get_tft_color(s,1), get_tft_color(s,2) );
    return "";
}

if(cmd=="tft.cursor") { // Координаты курсора
    tft.setCursor( PARG0(s,1), PARG0(s,2) );
    return "";
}

if(cmd=="tft.print") { // Цвет шрифта
    String txt = PARG_OTHER(s,0);
    txt.replace("\\n","\n");
    tft.print( txt );
    return "";
}

if(cmd == "tft.text") {  // Выводим текст по координатам
    tft.setCursor( PARG0(s,1), PARG0(s,2) );
    String txt = PARG_OTHER(s,2);
    txt.replace("\\n","\n");
    tft.print( txt );
    return "";
}



// Т О Ч К И    И    Л И Н И И

if(cmd == "tft.pixel") {  // Рисуем пиксель
    tft.drawPixel(PARG0(s,1), PARG0(s,2), get_tft_color(s, 3));
    return "";
}

if(cmd == "tft.writepixel") {  // Записываем пиксель
    tft.writePixel(PARG0(s,1), PARG0(s,2), get_tft_color(s,3));
    return "";
}

if(cmd == "tft.line") {  // Рисуем линию
    tft.drawLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return "";
}

if(cmd == "tft.fastvline") {  // Быстрая вертикальная линия
    tft.drawFastVLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return "";
}

if(cmd == "tft.fasthline") {  // Быстрая горизонтальная линия
    tft.drawFastHLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return "";
}

if(cmd == "tft.writefastvline") {  // Быстрая вертикальная линия (запись)
    tft.writeFastVLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return "";
}

if(cmd == "tft.writefasthline") {  // Быстрая горизонтальная линия (запись)
    tft.writeFastHLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return "";
}

if(cmd == "tft.writeline") {  // Линия (оптимизированная запись)
//    tft.writeLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    tft.drawLine(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return "";
}



// Ф И Г У Р Ы


if(cmd == "tft.rect") {  // Рисуем прямоугольник
    tft.drawRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s, 5));
    return "";
}

if(cmd == "tft.fillrect") {  // Заполняем прямоугольник
    uint16_t color = get_tft_color(s, 5);
    tft.fillRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s, 5));
    return "";
}

if(cmd == "tft.writefillrect") {  // Быстро заполняем прямоугольник
    tft.writeFillRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return "";
}

if(cmd == "tft.circle") {  // Рисуем круг
    tft.drawCircle(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return "";
}

if(cmd == "tft.fillcircle") {  // Заполняем круг
    tft.fillCircle(PARG0(s,1), PARG0(s,2), PARG0(s,3), get_tft_color(s,4));
    return "";
}

if(cmd == "tft.triangle") {  // Рисуем треугольник
    tft.drawTriangle(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), PARG0(s,6), get_tft_color(s,7));
    return "";
}

if(cmd == "tft.filltriangle") {  // Заполняем треугольник
    tft.fillTriangle(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), PARG0(s,6), get_tft_color(s,7));
    return "";
}

if(cmd == "tft.circlehelper") {  // Круговой помощник
    tft.drawCircleHelper(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), get_tft_color(s,5));
    return "";
}

if(cmd == "tft.fillcirclehelper") {  // Заполняемый круговой помощник
    tft.fillCircleHelper(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), get_tft_color(s,6));
    return "";
}

if(cmd == "tft.roundrect") {  // Рисуем скругленный прямоугольник
    tft.drawRoundRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), get_tft_color(s,6));
    return "";
}

if(cmd == "tft.fillroundrect") {  // Заполняем скругленный прямоугольник
    tft.fillRoundRect(PARG0(s,1), PARG0(s,2), PARG0(s,3), PARG0(s,4), PARG0(s,5), get_tft_color(s,6));
    return "";
}

    // void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
    if(cmd == "tft.char") {  // Выводим символ x, y,CHAR, color, bg, size
        tft.drawChar(PARG0(s,1), PARG0(s,2), (char)PARG(s,3)[0], get_tft_color(s,4), get_tft_color(s,5), PARG0(s,6));
        return "";
    }

    // void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
    if(cmd == "tft.charxy") {  // Выводим символ с разными размерами по X и Y
        tft.drawChar(PARG0(s,1), PARG0(s,2), (char)PARG(s,3)[0], get_tft_color(s,4), get_tft_color(s,5), PARG0(s,6), PARG0(s,7));
        return "";
    }


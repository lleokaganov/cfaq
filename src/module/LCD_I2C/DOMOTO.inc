// LCD 1602

/// LCD.init 48 1
if(CMD_EQ("LCD.init")) {
    uint8_t addr = PARG0(s,1," ",0x27); // I2C-адрес на шине: 0x27, 0x20 или 0x3F;
    uint8_t cols = PARG0(s,2," ",16); // символов в строке: 16
    uint8_t rows = PARG0(s,3," ",2);  // строк дисплея: 2

    if(lcd != nullptr) delete lcd;  // Освобождаем память, если объект уже существует
    lcd = new LiquidCrystal_I2C(addr, cols, rows); // Создаем объект динамически
    lcd->init();
    lcd->backlight(); // и сразу включаем подсветку
    // lcd->clear();
    // lcd->home();
    // lcd->cursor();
    // lcd->blink();
    // lcd->print("WWWWWWWW");

    // Serial.println("LCD initialized: "+HH0X(addr)+", "+String(cols)+"x"+String(rows));
    return EMPTY_STRING;
}

/// LCD.clear
if(CMD_EQ("LCD.clear")) {
    lcd->clear();
    return EMPTY_STRING;
}

/// LCD.light
if(CMD_EQ("LCD.light")) {
    if( PARG0(s,1," ",1) ) lcd->backlight(); // включить подсветку
    else lcd->noBacklight(); // выключить подсветку
    return EMPTY_STRING;
}

/// LCD.print
if(CMD_EQ("LCD.print")) {
    String txt = PARG_OTHER(s,0);
    txt.replace("\\n","\n");
    lcd->print(txt);
    return EMPTY_STRING;
}

/// LCD.cursor
if(CMD_EQ("LCD.cursor")) {
    lcd->setCursor( PARG0(s,1), PARG0(s,2) );
    return EMPTY_STRING;
}

/// LCD.show
/// Включает/выключает видимый курсор
if(CMD_EQ("LCD.show")) {
    if( PARG(s,1) ) lcd->cursor(); else lcd->noCursor();
    return EMPTY_STRING;
}

/// LCD.blink
/// Включает/выключает мигание курсора
if(CMD_EQ("LCD.blink")) {
    if( PARG(s,1) ) lcd->blink(); else lcd->noBlink();
    return EMPTY_STRING;
}

/// LCD.scroll LEFT/RIFHT
/// Сдвигать содержимое
if(CMD_EQ("LCD.scroll")) {
    if( PARG(s,1) == "LEFT" ) lcd->scrollDisplayLeft();
    else lcd->scrollDisplayRight();
    return EMPTY_STRING;
}

/// LCD.char.set 0 0x1F 0x11 0x11 0x11 0x11 0x11 0x1F 0x00 // рамка
/// LCD.char.set 1 0x00 0x00 0x04 0x0E 0x1F 0x04 0x00 0x00 // стрелка вверх
/// LCD.char.set 2 0x00 0x00 0x04 0x04 0x04 0x00 0x00 0x00 // палочка
/// LCD.char.set 3 0x00 0x00 0x00 0x04 0x04 0x00 0x00 0x00 // точка
/// Создаёт символ (до 8 штук), байты рисунка 8 строк по 5 бит
if(CMD_EQ("LCD.char.set")) {
    uint8_t buf[8]; for (int i=0; i < 8; i++) buf[i] = PARG0(s,i+2," ",0);
    lcd->createChar(PARG0(s,1), buf);
    return EMPTY_STRING;
}

/// LCD.char.img 0 .###. .#.#. .###. .#.#. .###. ..... .....
/// Создаёт символ (до 8 штук) из рисунка: 8 строк по 5 бит
if(CMD_EQ("LCD.char.set")) {
    uint8_t buf[8];
    for (int row = 0; row < 8; row++) {
        String line = PARG(s, row+2, " ", ".....");  // строка вида .###.
        uint8_t val = 0;
        for (int i=0; i<5; i++) { val <<= 1; if(line[i] == '#') val |= 1; }
        buf[row] = val;
    }
    lcd->createChar(PARG0(s,1), buf);
    return EMPTY_STRING;
}

/// LCD.char 0
/// Печатает символ
if(CMD_EQ("LCD.char")) {
    lcd->write( PARG0(s,1) );
    return EMPTY_STRING;
}

// /*
// LCD.display	LCD.display 1 / LCD.display 0	Включает/выключает видимость символов (без стирания). Аналог lcd->display() и lcd->noDisplay()
// LCD.home	LCD.home	Перемещает курсор в начало (0,0) без очистки экрана
// LCD.createChar	LCD.createChar 0 0x1F 0x1B 0x11 0x1B 0x1F 0x00 0x00 0x00	Создаёт символ (до 8 штук), передаёшь байты рисунка (8 строк по 5 бит)
// LCD.writeChar	LCD.writeChar 0	Печатает созданный пользовательский символ
// */

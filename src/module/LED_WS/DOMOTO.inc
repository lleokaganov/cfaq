/// LEDWS.init 48 1
if(cmd=="LEDWS.init") {
    uint8_t pin = PARG0(s,1," ",48); // 48 DEFAULT_LED_PIN;
    uint16_t numLeds = PARG0(s,2," ",1); // 1 количество светодиодов
    if(led_ws != nullptr) delete led_ws;  // Освобождаем память, если объект уже существует
    led_ws = new Adafruit_NeoPixel(numLeds, pin, NEO_GRB + NEO_KHZ800); // Создаем объект динамически
    led_ws->begin();
    led_ws->show(); // Гасим все светодиоды
    return "";
}

/// LEDWS 255 255 255 [0]
if(cmd=="LEDWS") {
    uint16_t numLed = PARG0(s,4); // номер светодиод
    uint8_t R = PARG0(s,1); // RED
    uint8_t G = PARG0(s,2); // GREEN
    uint8_t B = PARG0(s,3); // BLUE
    led_ws->setPixelColor( numLed, led_ws->Color(R, G, B) ); // цвет
    led_ws->show();
    return "";
}

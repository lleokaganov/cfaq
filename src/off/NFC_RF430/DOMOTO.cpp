/// I2C
/// ESP32 (основной I²C)	GPIO21, GPIO22
/// ESP8266 (основной I²C)	GPIO4 (D2), GPIO5 (D1)


/// Wire.begin(
/// RF430_init 3 46
/// Какие пины можно использовать для I²C на ESP32 / ESP8266?
/// ESP32 (любые пины, кроме GPIO6-11) Стандартные SDA = 21, SCL = 22
/// ESP8266 (нельзя GPIO0, GPIO15, GPIO16) Стандартные SDA = 4, SCL = 5

/*
// extern "C" {
 #include <driver/i2c.h>
// }

#define I2C_PORT I2C_NUM_0
*/

/// RF430_init {reset_pin} {irq_pin} [{i2c_SDA} {i2c_SCL}]
if(cmd=="RF430_init") {

    int SDA = PARG0(s,3);
    int SCL = PARG0(s,4);
    #ifdef ESP32
//	if(PARG(s,3)=="") SDA=21;
//	if(PARG(s,4)=="") SCL=22;
	if(PARG(s,3)=="") SDA=8;
	if(PARG(s,4)=="") SCL=9;
    #else
	if(PARG(s,3)=="") SDA=4;
	if(PARG(s,4)=="") SCL=5;
    #endif

int try_try = 10;

do {
    Wire.end(); // Завершаем текущую I²C-шину
    Wire.begin( SDA, SCL );
    // RF430
    if(rf430_nfc != nullptr) delete rf430_nfc;  // Освобождаем память, если объект уже существовал
    rf430_nfc = new RF430( PARG0(s,1) , PARG0(s,2) );  // Динамически создаем объект NFC
    delay(100);
    if( rf430_nfc->begin() ) break;

    ERR("driver Wire reinstall #"+String(11-try_try) );

    // Переводим SDA и SCL в обычный GPIO-режим перед отключением Wire
    gpio_reset_pin((gpio_num_t)SDA);
    gpio_reset_pin((gpio_num_t)SCL);
    pinMode(SDA, INPUT_PULLUP);
    pinMode(SCL, INPUT_PULLUP);
    // Проверяем, не завис ли SDA в LOW (устройство зажало линию)
    if(digitalRead(SDA) == LOW) {
        // Serial.println("SDA зажата, принудительный сброс I2C...");
        pinMode(SCL, OUTPUT);
        // Генерируем 9 тактов на SCL, чтобы "расшатать" зависшее устройство
        for(int i = 0; i < 9; i++) {
            digitalWrite(SCL, HIGH);
            delayMicroseconds(5);
            digitalWrite(SCL, LOW);
            delayMicroseconds(5);
        }
        // Освобождаем SDA
        pinMode(SDA, INPUT_PULLUP);
        delay(100);
    }

    // Проверяем, установлен ли драйвер перед удалением
    if(i2c_driver_delete(I2C_PORT) != ESP_OK) {
        // Serial.println("Предупреждение: I2C-драйвер не был установлен или уже удален.");
    }
    // i2c
    delay(100);  // Даем время на завершение
} while(--try_try);
    rf430_nfc->enable();
    return "";
}

/// Записать в метку данные (еслинет аргументов, то перезаписать последние)
/// RF430_write url https://site.lleo.me/messager10/index.html?code=00000000000000&int=4&foo=boo
/// RF430_write my some test
if(cmd=="RF430_write") {
    String txt = PARG(s,1);
    bool urlmode = 0;
    if( txt == "" ) { // гоним старые
	txt = RF430_outbox;
	urlmode = RF430_urlmode;
    } else {
        if( txt != "url" ) txt = PARG_OTHER(s,0);
	else { txt = PARG_OTHER(s,1); urlmode = 1; }
        // запомним
	RF430_outbox = txt;
	RF430_urlmode = urlmode;
    }
    if(rf430_nfc == nullptr) { ERR("nfc 01"); return "";  }
    rf430_nfc->flush();
    int buf_size = txt.length()+1; // +1 для NUL-терминатора
    uint8_t *buf = new uint8_t[buf_size];  // Динамическое выделение памяти
    if(!buf) { ERR("nfc 02"); return ""; }
    if(urlmode) {
        NDEF_URI uri; // Создаем объект NDEF_URI
        uri.setPayloadBuffer(buf, buf_size); // Устанавливаем буфер
        uri.setURI(txt.c_str()); // Записываем URL
        rf430_nfc->setDataLength( uri.sendTo(*rf430_nfc) );  // Устанавливаем размер данных
    } else {
        NDEF_TXT text("en");  // Создаем NDEF-объект
        text.setPayloadBuffer(buf, buf_size);  // Устанавливаем буфер
        text.setText(txt.c_str());
        rf430_nfc->setDataLength( text.sendTo(*rf430_nfc) );
    }
    delete[] buf;  // Освобождаем память
    rf430_nfc->enable();
    return "";
}

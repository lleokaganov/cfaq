#include <Wire.h>

#include "RF430CL.h"
// #include "RF430CL.cpp"
#include "NDEF.h"
// #include "NDEF.cpp"
#include "NDEF_TXT.h"
// #include "NDEF_TXT.cpp"
#include "NDEF_URI.h"
// #include "NDEF_URI.cpp"

extern "C" {
 #include <driver/i2c.h>
}

#define I2C_PORT I2C_NUM_0  // Основной I2C-контроллер ESP32













/// файл "/RF430_onInbox_callback" вызывается когда кто-то записал данные в метку
#define RF430_onInbox_cb "/RF430_onInbox_callback"

/// файл "/RF430_onOutbox_callback" вызывается если кто-то прочел метку
#define RF430_onOutbox_cb "/RF430_onOutbox_callback"

/// файл "/RF430_onError_callback" вызывается если случилась ошибка
#define RF430_onError_cb "/RF430_onError_callback"

RF430 *rf430_nfc = nullptr;  // Используем указатель, чтобы динамически выделять память

// последние переданные
String RF430_outbox = "";
bool RF430_urlmode = 0;
// полученное непрочитанное
String RF430_inbox = "";
// флаги
bool RF430_is_outbox = 0; // кто-то прочел


String RF430_get() {
    uint16_t len = rf430_nfc->getDataLength();
    if(len == 0 || len >= 512) return "ERROR LEN";
    NDEF_TXT msg;
    uint8_t *NDEF_BUFF = new uint8_t[len];
    if(NDEF_BUFF == nullptr) return "ERROR RAM";
    msg.setPayloadBuffer(NDEF_BUFF, len); // Устанавливаем буфер
    int ret = msg.import(*rf430_nfc);
    String s="ERROR LEN";
    if(ret > 0 && ret < 512) { NDEF_BUFF[ret]=0; s = msg.getText(); }
    delete[] NDEF_BUFF;
    return s;
}

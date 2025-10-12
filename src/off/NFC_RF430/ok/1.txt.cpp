RF430 *nfc = nullptr;  // Используем указатель, чтобы динамически выделять память
String RF430_last_message = "";
String RF430_last_mode = "";

int kum = 0;

void RF430_init(int reset_pin, int irq_pin) {
    if(nfc != nullptr) delete nfc;  // Освобождаем память, если объект уже существовал
    nfc = new RF430(reset_pin, irq_pin);  // Динамически создаем объект NFC
    nfc->begin();
    nfc->enable();
}

void setup() {
  Serial.begin(115200);
  delay(700);
  Wire.begin();
  RF430_init(RESET_PIN, IRQ_PIN);
  // RF430_set("Murri mirri "+String(++kum));
  RF430_set("https://site.lleo.me/messager10/index.html?code=00000000000000&int=4&foo=boo","url");
  Serial.println("\n\n ready ");
}

void loop() {
    if( nfc != nullptr && nfc->loop() ) {
        Serial.println("event");
        if(nfc->available()) { Serial.println("Read: ["+ RF430_get() +"]"); RF430_set(); }
        if(nfc->wasRead()) { Serial.println("READ ["+RF430_last_message+"]"); RF430_set("Murri mirri "+String(++kum)); } // delay(100); nfc->enable(); }
        if(nfc->isError()) { Serial.println("ERROR"); delay(100); nfc->enable(); }
    }
}

String RF430_get() {
    uint16_t len = nfc->getDataLength();
    if(len == 0 || len >= 512) return "ERROR LEN";
    NDEF_TXT msg;
    uint8_t *NDEF_BUFF = new uint8_t[len];
    if(NDEF_BUFF == nullptr) return "ERROR RAM";
    msg.setPayloadBuffer(NDEF_BUFF, len); // Устанавливаем буфер
    int ret = msg.import(*nfc);
    String s="ERROR LEN";
    if(ret > 0 && ret < 512) { NDEF_BUFF[ret]=0; s = msg.getText(); }
    delete[] NDEF_BUFF;
    return s;
}

void RF430_set() { RF430_set(RF430_last_message,RF430_last_mode); }
void RF430_set(String s) { RF430_set(s,"text"); }
void RF430_set(String s, String mode) {
    RF430_last_message = s;
    RF430_last_mode = mode;
    if(nfc == nullptr) return;
    nfc->flush();
    int buf_size = s.length()+1; // +1 для NUL-терминатора
    uint8_t *buf = new uint8_t[buf_size];  // Динамическое выделение памяти
    if(!buf) return;
    if(mode=="text") {
        NDEF_TXT text("en");  // Создаем NDEF-объект
        text.setPayloadBuffer(buf, buf_size);  // Устанавливаем буфер
        text.setText(s.c_str());
        nfc->setDataLength( text.sendTo(*nfc) );
    } else {
        NDEF_URI uri; // Создаем объект NDEF_URI
        uri.setPayloadBuffer(buf, buf_size); // Устанавливаем буфер
        uri.setURI(s.c_str()); // Записываем URL
        nfc->setDataLength( uri.sendTo(*nfc) );  // Устанавливаем размер данных
    }
    delete[] buf;  // Освобождаем память
    nfc->enable();
}

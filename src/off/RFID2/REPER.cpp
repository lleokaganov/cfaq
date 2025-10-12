
if(l == F("RFID2.card")) {
    if(!Serial.available()) return ""; // Нет данных — сразу выход
    String S="";
    int k=3;
    while(--k) {
        while(Serial.available()) S += (char)Serial.read();
        if(S[0] != 0x02) return ""; // Неверный стартовый байт, тогда без разницы, что мы там прочли
        if(S.length() > 2 && S.length() >= S[1]) { // Дошли до всей длины пакета
            int len = S[1];
            if(S[len-1] != 0x03) return ""; // Неверный завершающий байт
            // Проверка BCC
            int bcc = 0;
            String O = "";
            for(int i=1; i<len-2; i++) { // считаем сразу и bcc и строку тип карты + номер карты
                bcc ^= (uint8_t)S[i];
		if(i>=2) {
            	    if(S[i] < 16) O+="0"; // Добавить ведущий 0
            	    O+=String((uint8_t)S[i],HEX);
		}
            }
            if((uint8_t)S[len - 2] != bcc) return ""; // BCC не сошелся
            O.toUpperCase(); // Делаем заглавные буквы
            return O;
        }
        delay(10); // Подождать 10 мс и продолжить
    }
    return ""; // Таймаут
}

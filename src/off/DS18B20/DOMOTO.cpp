
 if(cmd=="DS18B20.begin") { // запустить
    int pin = PARG0(s,1);
    // Функция инициализации датчика с динамическим пином
    // Удаляем старые объекты, если они существуют
    if(oneWire != nullptr) {
        delete oneWire;
        oneWire = nullptr;
    }
    if(DS18B20 != nullptr) {
        delete DS18B20;
        DS18B20 = nullptr;
    }
    // Создаем новые объекты с указанным пином
    oneWire = new OneWire(pin);
    DS18B20 = new DallasTemperature(oneWire);

    // Инициализируем датчик
    DS18B20->begin();
    // DS18B20.begin();
    return LOG(cmd+" pin:"+String(pin));
 }


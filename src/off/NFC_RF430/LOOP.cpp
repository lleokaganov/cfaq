
    if( rf430_nfc != nullptr && rf430_nfc->loop() ) {

        if(rf430_nfc->available()) { // нам записали новые данные
		RF430_inbox = RF430_get(); // запомним их
		if(is_file(RF430_onInbox_cb)) MOTO(RF430_onInbox_cb); // если есть callback - вызвать
		// RF430_set(); // восстановить старые
		rf430_nfc->flush(); // на всякий случай
		rf430_nfc->enable(); // на всякий случай
	}

        if(rf430_nfc->wasRead()) {
		RF430_is_outbox = 1; // флажок, что нас прочли
		if(is_file(RF430_onOutbox_cb)) MOTO(RF430_onOutbox_cb); // если есть callback - вызвать
		rf430_nfc->enable(); // на всякий случай
	}

        if(rf430_nfc->isError()) {
		if(is_file(RF430_onError_cb)) MOTO(RF430_onError_cb); // если есть callback - вызвать
		else ERR(F("ERROR"));
		delay(100);
		rf430_nfc->flush(); // на всякий случай
		rf430_nfc->enable(); // на всякий случай
	}

    }

/// файл "/RF430_onInbox_callback" вызывается когда кто-то записал данные в метку
#define RF430_onInbox_cb "/RF430_onInbox_callback"

/// файл "/RF430_onOutbox_callback" вызывается если кто-то прочел метку
#define RF430_onOutbox_cb "/RF430_onOutbox_callback"

/// файл "/RF430_onError_callback" вызывается если случилась ошибка
#define RF430_onError_cb "/RF430_onError_callback"

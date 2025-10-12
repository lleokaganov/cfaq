
    /// наc кто-то прочел? 1 или 0
    /// if {RF430_IS_OUTBOX} == 1 echo Somebody read NFC!
    if(l == F("RF430_IS_OUTBOX")) {
	String s = String(RF430_is_outbox);
	RF430_is_outbox = 0;
	return s;
    }

    /// Вот что мы раздаем сейчас
    /// echo {RF430_OUTBOX}
    if(l == F("RF430_OUTBOX")) {
	return RF430_outbox;
    }

    /// прислали непрочитанное или ""
    /// set txt = {RF430_INBOX}
    /// if.!empty {txt} echo RECIEVED: {txt}
    if(l == F("RF430_INBOX")) {
	if(rf430_nfc == nullptr) {
	    if(is_file(RF430_onError_cb)) MOTO(RF430_onError_cb);
	    return F("error");
	}
	String text = RF430_inbox;
	RF430_inbox = "";
	return text;
    }

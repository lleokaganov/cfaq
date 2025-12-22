if(l == F("touch")) {
	uint16_t x, y;
	if (tft.getTouch(&x, &y)) return String(x) + " " + String(y);
	else return "";
}
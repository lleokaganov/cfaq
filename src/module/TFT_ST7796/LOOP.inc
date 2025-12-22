// static uint32_t mls;
// if (millis() - mls > 1000) {
//     mls = millis();
//     Serial.println("touchMap size: " + String(touchMap.size()) + " empty: " + String(touchMap.empty() ? "true" : "false"));
// }

static uint16_t touch_x, touch_y;
static String touch_last_key="";
static uint32_t touch_last_millis=0;
if (!touchMap.empty() && tft.getTouch(&touch_x, &touch_y)) {

    // Serial.println("Touch at: " + String(touch_x) + "," + String(touch_y));
    for (auto& b : touchMap) {
        if (touch_x >= b.x && touch_x < b.x+b.w && touch_y >= b.y && touch_y < b.y+b.h
            && ( b.value != touch_last_key || millis() - touch_last_millis > 200)
        ) {
            // Serial.println("Touch: " + String(b.value) + " (" + String(b.index) + ")");
            if (b.fn) b.fn(b.value, touch_x, touch_y);
            touch_last_key = b.value;
            touch_last_millis = millis();
        }
    }
}

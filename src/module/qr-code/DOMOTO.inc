// #include "h.hpp"

if (cmd =="tft.QR.HEX") {
    int x0 = PARG0(s, 1);
    int y0 = PARG0(s, 2);
    int scale = PARG0(s, 3);
    String hex = PARG_OTHER(s, 3);
    int len = hex.length() / 2;
    uint8_t* data = (uint8_t*) malloc(len);
    for (int i = 0; i < len; i++) {
        char buf[3] = { hex[2 * i], hex[2 * i + 1], 0 };
        data[i] = strtoul(buf, nullptr, 16);
    }
    draw_qr(x0, y0, scale, data, len);
    free(data);
    return EMPTY_STRING;
}

if (cmd =="tft.QR") {
    int x0 = PARG0(s, 1);
    int y0 = PARG0(s, 2);
    int scale = PARG0(s, 3);
    String text = PARG_OTHER(s, 3);
    int len = text.length();
    uint8_t* data = (uint8_t*) malloc(len);
    for (int i = 0; i < len; i++) {
        data[i] = (uint8_t)text[i];
    }
    draw_qr(x0, y0, scale, data, len);
    free(data);
    return EMPTY_STRING;
}
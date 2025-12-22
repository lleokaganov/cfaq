#include "h.hpp"

static uint8_t qr[qrcodegen_BUFFER_LEN_FOR_VERSION(6)];

void draw_qr(int x0, int y0, int scale, uint8_t *data, int len) {

    uint8_t buf[256];                 // ← вот эта строка у тебя отсутствовала
    memcpy(buf, data, len);           // ← копируем, потому что encodeBinary меняет буфер

    bool ok = qrcodegen_encodeBinary(
        buf, len,         // dataAndTemp, dataLen
        qr,               // qrcode
        qrcodegen_Ecc_LOW,
        qrcodegen_VERSION_MIN,
        6,
        qrcodegen_Mask_AUTO,
        true
    );

    if (!ok) return;

    int size = qrcodegen_getSize(qr);

    int color = get_tft_color("black",0);

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            bool dot = qrcodegen_getModule(qr, x, y);
            if (!dot) continue;
            tft.fillRect(x0 + x * scale, y0 + y * scale, scale, scale, color);
        }
    }
}

if(l == F("KY-IIC-3V3")
|| l == F("KY-IIC-3V3_P")
|| l == F("KY-IIC-3V3_T")) {
    uint8_t I2C_buf[6];

    // Торжественные пассы руками и плясы с бубном перед измерением
    if( ! I2C_read(0x78, 0xD0, 1, I2C_buf) ) return "error";
    usleep(15000);
    if( 0 != I2C_write(0x78, 0x30, "0x0A") ) return "error";
    usleep(15000);

    // Измерение
    if( ! I2C_read(0x78, 0x06, 6, I2C_buf) ) return "error";

    // Проверка ответа
    if (I2C_buf[0] != 0x04) return "error 04";

    uint32_t pressure = ((uint32_t)I2C_buf[1] << 16) |((uint32_t)I2C_buf[2] << 8) | ((uint32_t)I2C_buf[3]);
    if (l == F("KY-IIC-3V3_P")) return String(pressure);

    uint16_t temperature = ((uint16_t)I2C_buf[4] << 8) | ((uint16_t)I2C_buf[5]);
    if (l == F("KY-IIC-3V3_T")) return String(temperature);

    return String(pressure) + " " + String(temperature);
}

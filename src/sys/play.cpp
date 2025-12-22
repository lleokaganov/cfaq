#include <Arduino.h>
#include "main.h"

#ifdef ESP32
    bool pwmChannelUsed[MAX_PWM_CHANNELS] = {false};  // Флаги занятости каналов
    // Получаем первый свободный PWM-канал
    int getFreePWMChannel() {
        for(int i=0; i<MAX_PWM_CHANNELS; i++) {
            if(!pwmChannelUsed[i]) { pwmChannelUsed[i] = true; return i; }
        }
        return -1; // Все каналы заняты
    }
#endif

void play(String x) { String pin = CF("sound.pin"); if(pin != "") play(x, pin.toInt()); }

void play(String x, byte pin) {
    // LOGI(LOG_MAIN, "play: " + String(pin) + " " + x);
    const String NotaName = "DRMFSLCdrmfslc";
    const float NotaFreq[] = {261.63, 293.67, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25, 587.33, 659.26, 698.46, 783.99, 880.00, 987.77};

    byte soundpin0 = CF0("sound.pin.0");
    uint16_t DT = 200;
    pinMode(pin, OUTPUT);

    #ifdef ESP32
    int pwmChannel = getFreePWMChannel();  // Получаем свободный канал
    if(pwmChannel == -1) { LOGI(LOG_MAIN, "Ошибка: нет свободных PWM-каналов!"); return; }

    // Настраиваем таймер
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_12_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 2000,  // Стартовая частота
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Настраиваем канал
    ledc_channel_config_t ledc_channel = {
        .gpio_num = pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = (ledc_channel_t)pwmChannel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0, // Начальный уровень звука 0
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);
    #endif

    for (byte i = 0; i < x.length(); i++) {
        if (x[i] == '1') DT = 20;
        else if (x[i] == '2') DT = 50;
        else if (x[i] == '3') DT = 100;
        else if (x[i] == '4') DT = 200;
        else if (x[i] == '5') DT = 300;
        else if (x[i] == '6') DT = 500;
        else if (x[i] == '7') DT = 1000;
        else if (x[i] == '8') DT = 2000;
        else if (x[i] == '.') { digitalWrite(pin, soundpin0); delay(DT); }
        else if (x[i] == ',') { digitalWrite(pin, soundpin0); delay(DT / 2); }
        else {
            for (byte j = 0; j < NotaName.length(); j++) {
                if (x[i] == NotaName[j]) {
                #ifdef ESP32
                    ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, NotaFreq[j]);  // Устанавливаем частоту
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)pwmChannel, 128);
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)pwmChannel);
                    delay(DT);
                    ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)pwmChannel, 0);
                    ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)pwmChannel);
                #else
                    tone(pin, NotaFreq[j], DT);
                    delay(DT);
                    noTone(pin);
                #endif
                    break;
                }
            }
        }
    }

    #ifdef ESP32
	// Убираем звук перед отключением PWM
	ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)pwmChannel, 0);
	ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)pwmChannel);
	// Полностью отключаем PWM, чтобы убрать шум
	ledc_stop(LEDC_LOW_SPEED_MODE, (ledc_channel_t)pwmChannel, 0);
        pwmChannelUsed[pwmChannel] = false;  // Освобождаем канал
    #endif

    pinMode(pin, OUTPUT);
    digitalWrite(pin, soundpin0); // Принудительно отключаем пин
}

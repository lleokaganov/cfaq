#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// Определяем пины
  #define TFT_RST    38  // Reset (RES) RES  → Reset (RST)
  #define TFT_DC     37  // Data/Command (DC) DC   → Data/Command (DC)
  #define TFT_SCK    36  // Clock (SCK) SCL  → Clock (SCK)
  #define TFT_MOSI   35  // MOSI (DATA) SDA  → MOSI (DATA)

// CS   → Chip Select (CS)  
// BLK  → Подсветка (Backlight)  


// Создаём объект дисплея **без CS**
Adafruit_ST7789 tft = Adafruit_ST7789(-1, TFT_DC, TFT_RST);  // -1 означает, что CS не используется

void setup() {
    Serial.begin(115200);
    delay(2000);

    // Инициализация SPI без CS
    SPI.begin(TFT_SCK, -1, TFT_MOSI);  // (SCK, MISO, MOSI)

    // Инициализация дисплея
    // tft.init(240, 240);  // Разрешение 240x240 (или 320x240)
    // tft.init(320, 240);  // Разрешение 240x240 (или 320x240)
    tft.setRotation(1);  // Поворот экрана (0-3)
    tft.init(240, 280, SPI_MODE3);  // Иногда ST7789 работает в режиме SPI_MODE3
    tft.setRotation(1);  // Поворот экрана (0-3)

    // Очистка экрана
    tft.fillScreen(ST77XX_BLACK);
    
    // Выводим текст
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(20, 40);
    tft.print("Hello, ESP32-S3!");
    
    // tft.setCursor(10, 10);
    // tft.print("Hello, ESP32-S3!");

    // tft.setCursor(100, 100);
    // tft.print("Hello, ESP32-S3!");

    tft.drawRect(0, 0, 280, 240, ST77XX_RED);  // Белая рамка по краям
/*
// Some ready-made 16-bit ('565') color settings:
#define ST77XX_BLACK 0x0000
#define ST77XX_WHITE 0xFFFF
#define ST77XX_RED 0xF800
#define ST77XX_GREEN 0x07E0
#define ST77XX_BLUE 0x001F
#define ST77XX_CYAN 0x07FF
#define ST77XX_MAGENTA 0xF81F
#define ST77XX_YELLOW 0xFFE0
#define ST77XX_ORANGE 0xFC00
*/
}

void loop() {
  Serial.println("ping... "+String(tft.width())+"x"+String(tft.height()));
  delay(1000);
  // tft.print("Hello, ESP32-S3!");
}

#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BH1750 lightMeter;

void setup() {
    Serial.begin(9600);
    Wire.begin();

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 not found. Check wiring");
        while (true);
    }

    if (lightMeter.begin()) {
        Serial.println("BH1750 found and ready.");
    } else {
        Serial.println("BH1750 not found. Check wiring");
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Lux2ISO ready!");
    display.display();
    delay(1000);
}

void loop() {
    float lux = lightMeter.readLightLevel();

    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");

    display.clearDisplay();

    // Line 1: lux reading
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(lux, 1);
    display.println(" lx");

    // Line 2: label
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.println("Ambient Light");

    display.display();
    delay(1000);
}
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

const char* apertureStr(float n) {
    if (n <= 1.2)  return "f/1";
    if (n <= 1.5)  return "f/1.4";
    if (n <= 2.2)  return "f/2";
    if (n <= 3.0)  return "f/2.8";
    if (n <= 4.5)  return "f/4";
    if (n <= 6.5)  return "f/5.6";
    if (n <= 9.5)  return "f/8";
    if (n <= 12.5) return "f/11";
    if (n <= 18.0) return "f/16";
    return "f/22";
}

const char* shutterStr(float s) {
    if (s >= 0.9)   return "1s";
    if (s >= 0.4)   return "1/2";
    if (s >= 0.2)   return "1/4";
    if (s >= 0.09)  return "1/8";
    if (s >= 0.05)  return "1/15";
    if (s >= 0.025) return "1/30";
    if (s >= 0.012) return "1/60";
    if (s >= 0.006) return "1/125";
    if (s >= 0.003) return "1/250";
    if (s >= 0.0015) return "1/500";
    return "1/1000";
}

int snapISO(float iso_raw) {
    float stops = log(iso_raw / 100.0) / log(2.0);
    stops = round(stops);
    if (stops < 0) stops = 0;
    if (stops > 5) stops = 5;
    return (int)round(100.0 * pow(2, stops));
}

void loop() {
    float lux = lightMeter.readLightLevel();
    float aperture = 8.0;        // f/8 — common general-purpose aperture
    float shutter_speed = 0.008; // 1/125s — standard film camera shutter speed

    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");

    display.clearDisplay();

    // lux reading
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(lux, 1);
    display.println(" lx");

    // Convert To ISO
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.print("Aperture: ");
    display.println(apertureStr(aperture));
    display.setCursor(0, 30);
    display.print("Shutter: ");
    display.println(shutterStr(shutter_speed));
    display.setCursor(0, 40);
    if (lux <= 0.0) {
        display.println("ISO: --");
    } else {
        float iso_raw = (250.0 * aperture * aperture) / (lux * shutter_speed);
        int iso = snapISO(iso_raw);
        display.print("ISO: ");
        display.println(iso);
    }

    // light condition label
    display.setTextSize(1);
    display.setCursor(0, 55);
    if (lux >= 10000) {
        display.println("Direct Sunlight");
    } else if (lux >= 1000) {
        display.println("Bright Outdoors");
    } else if (lux >= 200) {
        display.println("Overcast / Shade");
    } else if (lux >= 50) {
        display.println("Indoor Bright");
    } else if (lux >= 10) {
        display.println("Indoor Dim");
    } else {
        display.println("Low Light / Night");
    }

    display.display();
    delay(500);
}
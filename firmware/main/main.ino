#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

#define BTN_SHUTTER  6   // SW1 — PD6 — cycles shutter speed
#define BTN_APERTURE 7   // SW2 — PD7 — cycles aperture

#define DEBOUNCE_MS  50

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BH1750 lightMeter;

// ── Aperture table ────────────────────────────────────────────────────────────
const float APERTURES[]    = { 1.0, 1.4, 2.0, 2.8, 4.0, 5.6, 8.0, 11.0, 16.0, 22.0 };
const char* APERTURE_STR[] = { "f/1", "f/1.4", "f/2", "f/2.8", "f/4",
                                "f/5.6", "f/8", "f/11", "f/16", "f/22" };
const uint8_t APERTURE_COUNT = sizeof(APERTURES) / sizeof(APERTURES[0]);

// ── Shutter speed table ───────────────────────────────────────────────────────
const float SHUTTERS[]    = { 1.0, 0.5, 0.25, 0.125, 1.0/15, 1.0/30,
                               1.0/60, 1.0/125, 1.0/250, 1.0/500, 1.0/1000 };
const char* SHUTTER_STR[] = { "1s", "1/2", "1/4", "1/8", "1/15", "1/30",
                               "1/60", "1/125", "1/250", "1/500", "1/1000" };
const uint8_t SHUTTER_COUNT = sizeof(SHUTTERS) / sizeof(SHUTTERS[0]);

// ── State ─────────────────────────────────────────────────────────────────────
uint8_t apertureIdx = 6;   // default f/8
uint8_t shutterIdx  = 7;   // default 1/125s

// Button state tracking
bool     lastShutterBtn  = HIGH;
bool     lastApertureBtn = HIGH;
uint32_t lastShutterMs   = 0;
uint32_t lastApertureMs  = 0;

// ── ISO snapping ──────────────────────────────────────────────────────────────
int snapISO(float iso_raw) {
    float stops = log(iso_raw / 100.0) / log(2.0);
    stops = round(stops);
    if (stops < 0) stops = 0;
    if (stops > 5) stops = 5;
    return (int)round(100.0 * pow(2.0, stops));
}

// ── Scene label ───────────────────────────────────────────────────────────────
const char* sceneLabel(float lux) {
    if (lux >= 10000) return "Direct Sunlight";
    if (lux >= 1000)  return "Bright Outdoors";
    if (lux >= 200)   return "Overcast / Shade";
    if (lux >= 50)    return "Indoor Bright";
    if (lux >= 10)    return "Indoor Dim";
    return "Low Light / Night";
}

// ── Button helper — returns true on a debounced falling edge ─────────────────
bool buttonPressed(uint8_t pin, bool &lastState, uint32_t &lastMs) {
    bool current = digitalRead(pin);
    if (current == LOW && lastState == HIGH) {
        uint32_t now = millis();
        if (now - lastMs >= DEBOUNCE_MS) {
            lastMs    = now;
            lastState = current;
            return true;
        }
    }
    lastState = current;
    return false;
}

// ── Setup ─────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(9600);
    Wire.begin();

    pinMode(BTN_SHUTTER,  INPUT_PULLUP);
    pinMode(BTN_APERTURE, INPUT_PULLUP);

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

// ── Loop ──────────────────────────────────────────────────────────────────────
void loop() {
    // Handle shutter speed button (SW1)
    if (buttonPressed(BTN_SHUTTER, lastShutterBtn, lastShutterMs)) {
        shutterIdx = (shutterIdx + 1) % SHUTTER_COUNT;
    }

    // Handle aperture button (SW2)
    if (buttonPressed(BTN_APERTURE, lastApertureBtn, lastApertureMs)) {
        apertureIdx = (apertureIdx + 1) % APERTURE_COUNT;
    }

    float lux      = lightMeter.readLightLevel();
    float aperture = APERTURES[apertureIdx];
    float shutter  = SHUTTERS[shutterIdx];

    Serial.print("Lux: ");
    Serial.print(lux, 1);
    Serial.print("  Aperture: ");
    Serial.print(APERTURE_STR[apertureIdx]);
    Serial.print("  Shutter: ");
    Serial.println(SHUTTER_STR[shutterIdx]);

    display.clearDisplay();

    // Row 0 — lux reading (large)
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(lux, 1);
    display.println(" lx");

    // Row 1 — aperture & shutter
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.print("Ap: ");
    display.print(APERTURE_STR[apertureIdx]);
    display.print("  Sh: ");
    display.println(SHUTTER_STR[shutterIdx]);

    // Row 2 — recommended ISO
    display.setCursor(0, 32);
    if (lux <= 0.0) {
        display.println("ISO: --");
    } else {
        float iso_raw = (250.0 * aperture * aperture) / (lux * shutter);
        display.print("ISO: ");
        display.println(snapISO(iso_raw));
    }

    // Row 3 — scene label
    display.setCursor(0, 48);
    display.println(sceneLabel(lux));

    display.display();
    delay(100);
}
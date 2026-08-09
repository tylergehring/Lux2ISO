#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1

// Encoder 1 — Shutter Speed (A=PD2/INT0, B=PD3/INT1, SW=PD4)
#define ENC1_A   2
#define ENC1_B   3
#define ENC1_SW  4

// Encoder 2 — Aperture (A=PD5, B=PD6, SW=PD7)
#define ENC2_A   5
#define ENC2_B   6
#define ENC2_SW  7

#define DEBOUNCE_MS  50
#define DISPLAY_MS   80   // display refresh interval

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

// Encoder quadrature last-state (bits: [1]=A [0]=B)
uint8_t enc1State = 0;
uint8_t enc2State = 0;

// Encoder switch debounce
bool     lastEnc1Sw  = HIGH;
bool     lastEnc2Sw  = HIGH;
uint32_t lastEnc1SwMs = 0;
uint32_t lastEnc2SwMs = 0;

// Quadrature lookup: +1=CW, -1=CCW, 0=no valid step
// Indexed by (lastAB << 2 | currAB)
static const int8_t ENC_TABLE[16] = {
    0,  1, -1,  0,
   -1,  0,  0,  1,
    1,  0,  0, -1,
    0, -1,  1,  0
};

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

// ── Quadrature read — returns +1, -1, or 0 ───────────────────────────────────
int8_t readEncoder(uint8_t pinA, uint8_t pinB, uint8_t &lastState) {
    uint8_t curr  = (digitalRead(pinA) << 1) | digitalRead(pinB);
    int8_t  delta = ENC_TABLE[(lastState << 2) | curr];
    lastState = curr;
    return delta;
}

// ── Debounced button — true on falling edge ───────────────────────────────────
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

    pinMode(ENC1_A,  INPUT_PULLUP);
    pinMode(ENC1_B,  INPUT_PULLUP);
    pinMode(ENC1_SW, INPUT_PULLUP);
    pinMode(ENC2_A,  INPUT_PULLUP);
    pinMode(ENC2_B,  INPUT_PULLUP);
    pinMode(ENC2_SW, INPUT_PULLUP);

    // Seed encoder state from current pin levels
    enc1State = (digitalRead(ENC1_A) << 1) | digitalRead(ENC1_B);
    enc2State = (digitalRead(ENC2_A) << 1) | digitalRead(ENC2_B);

    uint8_t oledAddr = 0x3C;
    bool oledOk = display.begin(SSD1306_SWITCHCAPVCC, oledAddr);
    if (!oledOk) {
        oledAddr = 0x3D;
        oledOk = display.begin(SSD1306_SWITCHCAPVCC, oledAddr);
    }
    if (!oledOk) {
        Serial.println("SSD1306 not found at 0x3C or 0x3D. Check wiring");
        while (true);
    }
    Serial.print("SSD1306 found at 0x");
    Serial.println(oledAddr, HEX);

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
    // Poll encoders every iteration so no detent is missed
    int8_t enc1Delta = readEncoder(ENC1_A, ENC1_B, enc1State);
    int8_t enc2Delta = readEncoder(ENC2_A, ENC2_B, enc2State);

    // Encoder 1 — Shutter Speed (CW = faster, CCW = slower)
    if (enc1Delta > 0 && shutterIdx  < SHUTTER_COUNT  - 1) shutterIdx++;
    if (enc1Delta < 0 && shutterIdx  > 0)                  shutterIdx--;

    // Encoder 2 — Aperture (CW = higher f-number, CCW = lower)
    if (enc2Delta > 0 && apertureIdx < APERTURE_COUNT - 1) apertureIdx++;
    if (enc2Delta < 0 && apertureIdx > 0)                  apertureIdx--;

    // Encoder switches — press to reset that value to default
    if (buttonPressed(ENC1_SW, lastEnc1Sw, lastEnc1SwMs)) shutterIdx  = 7; // 1/125s
    if (buttonPressed(ENC2_SW, lastEnc2Sw, lastEnc2SwMs)) apertureIdx = 6; // f/8

    // Refresh display and serial at a fixed interval; BH1750 updates ~every 120ms
    static uint32_t lastDisplay = 0;
    if (millis() - lastDisplay < DISPLAY_MS) return;
    lastDisplay = millis();

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

    // Row 1 — compact settings + ISO for 128x32
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.print("Ap: ");
    display.print(APERTURE_STR[apertureIdx]);
    display.print(" Sh:");
    display.print(SHUTTER_STR[shutterIdx]);
    display.print(" I:");
    if (lux <= 0.0) {
        display.print("--");
    } else {
        float iso_raw = (250.0 * aperture * aperture) / (lux * shutter);
        display.print(snapISO(iso_raw));
    }

    display.display();
}
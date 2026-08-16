#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1

// ── Pin assignments ────────────────────────────────────────────────────────────
#define ENC1_A    2   // PD2 INT0
#define ENC1_B    3   // PD3 INT1
#define ENC1_SW   4   // PD4 — shared: UI mode toggle (short) + shutdown (long)
#define PWR_HOLD  5   // PD5 — hold latch ON; drive LOW to cut system power

// ── Timing ────────────────────────────────────────────────────────────────────
#define DEBOUNCE_MS      50
#define LONG_PRESS_MS  5000   // hold duration before shutdown
#define DISPLAY_MS       80   // display refresh interval

// ── Edit mode: single encoder toggles between shutter and aperture ─────────────
enum EditMode { EDIT_SHUTTER, EDIT_APERTURE };
EditMode editMode = EDIT_SHUTTER;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BH1750 lightMeter;

// ── Aperture table ─────────────────────────────────────────────────────────────
const float APERTURES[]    = { 1.0, 1.4, 2.0, 2.8, 4.0, 5.6, 8.0, 11.0, 16.0, 22.0 };
const char* APERTURE_STR[] = { "f/1", "f/1.4", "f/2", "f/2.8", "f/4",
                                "f/5.6", "f/8", "f/11", "f/16", "f/22" };
const uint8_t APERTURE_COUNT = sizeof(APERTURES) / sizeof(APERTURES[0]);

// ── Shutter speed table ────────────────────────────────────────────────────────
const float SHUTTERS[]    = { 1.0, 0.5, 0.25, 0.125, 1.0/15, 1.0/30,
                               1.0/60, 1.0/125, 1.0/250, 1.0/500, 1.0/1000 };
const char* SHUTTER_STR[] = { "1s", "1/2", "1/4", "1/8", "1/15", "1/30",
                               "1/60", "1/125", "1/250", "1/500", "1/1000" };
const uint8_t SHUTTER_COUNT = sizeof(SHUTTERS) / sizeof(SHUTTERS[0]);

// ── State ─────────────────────────────────────────────────────────────────────
uint8_t apertureIdx = 6;   // default f/8
uint8_t shutterIdx  = 7;   // default 1/125s
uint8_t enc1State   = 0;

// ── Button state machine ───────────────────────────────────────────────────────
enum ButtonState { BTN_IDLE, BTN_PRESSED, BTN_HELD };
ButtonState btnState   = BTN_IDLE;
uint32_t    btnPressMs = 0;

// Quadrature lookup: +1=CW, -1=CCW, 0=no valid step
// Indexed by (lastAB << 2 | currAB)
static const int8_t ENC_TABLE[16] = {
    0,  1, -1,  0,
   -1,  0,  0,  1,
    1,  0,  0, -1,
    0, -1,  1,  0
};

// ── ISO snapping (whole-stop increments from ISO 100) ─────────────────────────
int snapISO(float iso_raw) {
    float stops = log(iso_raw / 100.0) / log(2.0);
    stops = round(stops);
    if (stops < 0) stops = 0;
    if (stops > 5) stops = 5;
    return (int)round(100.0 * pow(2.0, stops));
}

// ── Quadrature read — returns +1, -1, or 0 ───────────────────────────────────
int8_t readEncoder(uint8_t pinA, uint8_t pinB, uint8_t &lastState) {
    uint8_t curr  = (digitalRead(pinA) << 1) | digitalRead(pinB);
    int8_t  delta = ENC_TABLE[(lastState << 2) | curr];
    lastState = curr;
    return delta;
}

// ── Shutdown — quiesce peripherals then release latch ─────────────────────────
void shutdownSystem() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(22, 12);
    display.print("Powering off...");
    display.display();
    delay(800);

    // Release latch — VSYS drops to 0 V; MCU loses power within a few ms
    digitalWrite(PWR_HOLD, LOW);
    while (true);   // wait for rail to collapse; never returns
}

// ── Setup ─────────────────────────────────────────────────────────────────────
void setup() {
    // Assert PWR_HOLD before anything else so startup pulse can be released
    pinMode(PWR_HOLD, OUTPUT);
    digitalWrite(PWR_HOLD, HIGH);

    pinMode(ENC1_A,  INPUT_PULLUP);
    pinMode(ENC1_B,  INPUT_PULLUP);
    pinMode(ENC1_SW, INPUT_PULLUP);

    enc1State = (digitalRead(ENC1_A) << 1) | digitalRead(ENC1_B);

    Serial.begin(9600);
    Wire.begin();

    uint8_t oledAddr = 0x3C;
    bool oledOk = display.begin(SSD1306_SWITCHCAPVCC, oledAddr);
    if (!oledOk) {
        oledAddr = 0x3D;
        oledOk   = display.begin(SSD1306_SWITCHCAPVCC, oledAddr);
    }
    if (!oledOk) {
        Serial.println("SSD1306 not found at 0x3C or 0x3D");
        while (true);
    }
    Serial.print("SSD1306 @ 0x"); Serial.println(oledAddr, HEX);

    if (lightMeter.begin()) {
        Serial.println("BH1750 ready");
    } else {
        Serial.println("BH1750 not found — check wiring");
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Lux2ISO P3");
    display.setCursor(0, 12);
    display.println("Hold 5s to off");
    display.display();
    delay(1000);
}

// ── Loop ──────────────────────────────────────────────────────────────────────
void loop() {
    uint32_t now = millis();

    // Encoder rotation — adjusts whichever parameter is active
    int8_t enc1Delta = readEncoder(ENC1_A, ENC1_B, enc1State);
    if (editMode == EDIT_SHUTTER) {
        if (enc1Delta > 0 && shutterIdx  < SHUTTER_COUNT  - 1) shutterIdx++;
        if (enc1Delta < 0 && shutterIdx  > 0)                  shutterIdx--;
    } else {
        if (enc1Delta > 0 && apertureIdx < APERTURE_COUNT - 1) apertureIdx++;
        if (enc1Delta < 0 && apertureIdx > 0)                  apertureIdx--;
    }

    // Button state machine: short press = mode toggle, long press = shutdown
    bool btnDown = (digitalRead(ENC1_SW) == LOW);

    switch (btnState) {
        case BTN_IDLE:
            if (btnDown) {
                btnState   = BTN_PRESSED;
                btnPressMs = now;
            }
            break;

        case BTN_PRESSED:
            if (now - btnPressMs >= LONG_PRESS_MS) {
                btnState = BTN_HELD;
                shutdownSystem();  // does not return
            } else if (!btnDown) {
                // Released before long-press threshold — treat as mode toggle
                if (now - btnPressMs >= DEBOUNCE_MS) {
                    editMode = (editMode == EDIT_SHUTTER) ? EDIT_APERTURE : EDIT_SHUTTER;
                }
                btnState = BTN_IDLE;
            }
            break;

        case BTN_HELD:
            // shutdownSystem() loops forever; this branch is unreachable
            btnState = BTN_IDLE;
            break;
    }

    // Refresh display at fixed interval
    static uint32_t lastDisplay = 0;
    if (now - lastDisplay < DISPLAY_MS) return;
    lastDisplay = now;

    float lux      = lightMeter.readLightLevel();
    float aperture = APERTURES[apertureIdx];
    float shutter  = SHUTTERS[shutterIdx];

    Serial.print("Lux: ");    Serial.print(lux, 1);
    Serial.print("  Ap: ");   Serial.print(APERTURE_STR[apertureIdx]);
    Serial.print("  Sh: ");   Serial.print(SHUTTER_STR[shutterIdx]);
    Serial.print("  Mode: "); Serial.println(editMode == EDIT_SHUTTER ? "SH" : "AP");

    display.clearDisplay();

    // Row 0 — lux reading (large text)
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(lux, 1);
    display.println(" lx");

    // Row 1 — active parameter (>) + other parameter + ISO
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.print(editMode == EDIT_SHUTTER ? ">" : " ");
    display.print("Sh:");
    display.print(SHUTTER_STR[shutterIdx]);
    display.print(" ");
    display.print(editMode == EDIT_APERTURE ? ">" : " ");
    display.print(APERTURE_STR[apertureIdx]);
    display.print(" I:");
    if (lux <= 0.0) {
        display.print("--");
    } else {
        float iso_raw = (250.0 * aperture * aperture) / (lux * shutter);
        display.print(snapISO(iso_raw));
    }

    display.display();
}
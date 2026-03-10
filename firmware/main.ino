#include <wire.h>
#include <BH1750.h>

BH1750 lightMeter;

void setup() {
    Serial.begin(9600);
    Wire.begin();

    if (lightMeter.begin()) {
        Serial.printLn("BH1750 found and ready.");
    } else {
        Serial.printLn("BH1750 not found. Check wiring")
    }
}

void loop() {
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.print(" lx");

    delay(1000);
}
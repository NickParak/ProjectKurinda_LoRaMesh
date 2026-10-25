#include <Arduino.h>
#include <RadioLib.h>

constexpr uint8_t PIN_NSS  = 7;
constexpr uint8_t PIN_DIO1 = 3;
constexpr uint8_t PIN_RST  = 10;
constexpr uint8_t PIN_BUSY = 2;
constexpr uint8_t PIN_SCK  = 4;
constexpr uint8_t PIN_MISO = 5;
constexpr uint8_t PIN_MOSI = 6;

SX1262 radio = new Module(
    PIN_NSS,
    PIN_DIO1,
    PIN_RST,
    PIN_BUSY
);

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("DX-LR30 hardware test");

    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_NSS);

    radio.tcxoVoltage = 0.0;

    int16_t state = radio.begin();

    if (state == RADIOLIB_ERR_NONE) {
        Serial.println("SX1262 detected!");
    } else {
        Serial.print("RadioLib error: ");
        Serial.println(state);
    }
}

void loop() {

    Serial.println("Sending test packet...");
    delay(1000);
}

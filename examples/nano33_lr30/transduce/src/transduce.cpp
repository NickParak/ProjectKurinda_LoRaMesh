#include <Arduino.h>
#include <RadioLib.h>

constexpr uint8_t PIN_NSS  = 10;
constexpr uint8_t PIN_DIO1 = 2;
constexpr uint8_t PIN_RST  = 9;
constexpr uint8_t PIN_BUSY = 8;

SX1262 radio = new Module(
    PIN_NSS,
    PIN_DIO1,
    PIN_RST,
    PIN_BUSY
);

// save transmission state between loops
int transmissionState = RADIOLIB_ERR_NONE;

// flag to indicate that a packet was sent
volatile bool transmittedFlag = false;
volatile bool receivedFlag = false;

// this function is called when a complete packet
// is transmitted by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  // we sent a packet, set the flag
  transmittedFlag = true;
  receivedFlag = true;
}

void setup() {
    Serial.begin(9600);
    
    // initialize SX1262 at 434 MHz
    Serial.print(F("[SX1262] Initializing ... "));
    ConfigLoRa_t config;
    config.frequency = 434;
    int state = radio.begin(config);
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true) { delay(10); }
    }
    
    // set the function that will be called
    // when new packet is received
    radio.setPacketSentAction(setFlag);
    radio.setPacketReceivedAction(setFlag);

    transmissionState = radio.startTransmit("Nick Transduce Setup");
}

int sendCount = 0;
int receiveCount = 0;

void loop() {
    // check if the previous transmission finished
    if(transmittedFlag) {
        // reset flag
        transmittedFlag = false;

        if (transmissionState == RADIOLIB_ERR_NONE) {
            // packet was successfully sent
            Serial.println(F("transmission finished!"));
            
        } else {
            Serial.print(F("failed, code "));
            Serial.println(transmissionState);
        }

        radio.finishTransmit();
        delay(3000);
        Serial.print(F("[SX1262] Starting to listen ... "));
        int state = radio.startReceive();
        if (state == RADIOLIB_ERR_NONE) {
            Serial.println(F("success!"));
        } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true) { delay(10); }
        } 

        String str;
        state = radio.readData(str);
        if (state == RADIOLIB_ERR_NONE) {
            // packet was successfully received
            Serial.println(F("[SX1262] Received packet!"));

            // print data of the packet
            Serial.print(F("[SX1262] Data:\t\t"));
            Serial.println(str);
        } 
        else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
            // packet was received, but is malformed
            Serial.println(F("CRC error!"));
        } 
        else {
            // some other error occurred
            Serial.print(F("failed, code "));
            Serial.println(state);
        
        delay(3000);    
    }
}
    if(receivedFlag) {
        // reset flag
        receivedFlag = false;

        String str = "Nick Transduce #" + String(sendCount++);
        transmissionState = radio.startTransmit(str);        
        }
    }

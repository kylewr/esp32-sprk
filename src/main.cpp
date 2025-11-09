#include <Arduino.h>
#include <vector>

#include "SPISlaveWrapper.hpp"

#define LED 2

SPISlaveWrapper sprkSPI;

void setup() {
    Serial.begin(9600);
    delay(200);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    sprkSPI.begin();

    Serial.println("SPI Slave Initialized");
}

void loop() {
    sprkSPI.listenAsync();

    if (sprkSPI.hasNewCommand()) {
        Serial.print("New SPI Command Received: ");
        for (size_t i = 0; i < SPIMappings::BUFFER_SIZE; i++) {
            Serial.print(sprkSPI.getLatestCommand()[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");
        digitalWrite(LED, HIGH);
    }
    
    sprkSPI.parseCommand();
}

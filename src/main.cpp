#include <Arduino.h>
#include <vector>

#include "ModuleCollection.hpp"
#include "SPISlaveWrapper.hpp"

#include "modules/RSL.hpp"

#define LED 2

ModuleCollection mc;
SPISlaveWrapper sprkSPI;

void setup() {
    Serial.begin(9600);

    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    mc.addModule(new RSL());

    mc.initAll();
    sprkSPI.begin();
}

void loop() {
    mc.updateAll();

    sprkSPI.listenAsync();

    sprkSPI.parseCommand([](byte* command) {
        using namespace SPIMappings;

        Serial.print("Command Received: ");
        for (size_t i = 0; i < SPIMappings::BUFFER_SIZE; i++) {
            Serial.print(command[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");

        COMMAND_IDENT ident = identFromByte(command[0]);
        switch (ident) {
            case COMMAND_IDENT::NOP:
                // Do nothing
                break;
            case COMMAND_IDENT::LED_CTRL:
                if (command[1] != 0x00) {
                    digitalWrite(LED, HIGH);
                } else {
                    digitalWrite(LED, LOW);
                }
                break;
            case COMMAND_IDENT::RSL_STATE:
                // Use the second byte to determine the rsl blink state
                mc.getModule<RSL>(RSL::MODULE_NAME)->setState(command[1]);
                // use the next 3 bytes to form an integer blink rate in Hz
                uint32_t blinkRateHz = (command[2] << 16) | (command[3] << 8) | command[4];
                if (blinkRateHz != 0) {
                    mc.getModule<RSL>(RSL::MODULE_NAME)->setBlink(blinkRateHz);
                }
                break;
        }
    });
}

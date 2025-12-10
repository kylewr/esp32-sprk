#include <Arduino.h>
#include <vector>

#include "ModuleCollection.hpp"
#include "SPISlaveWrapper.hpp"

#include "modules/RSL.hpp"
#include "modules/Pinchers.hpp"

#include <ESP32Servo.h>

#define LED 2

ModuleCollection mc;
SPISlaveWrapper sprkSPI;

void setup() {
    Serial.begin(9600);

    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    mc.addModule(new RSL());
    mc.addModule(new Pinchers());

    mc.initAll();
    sprkSPI.begin();

    Serial.println("Setup complete");
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
            case COMMAND_IDENT::NOP: {
                // Do nothing
                break;
            }
            case COMMAND_IDENT::RESET: {
                ESP.restart();
                break;
            }
            case COMMAND_IDENT::LED_CTRL: {
                if (command[1] != 0x00) {
                    digitalWrite(LED, HIGH);
                } else {
                    digitalWrite(LED, LOW);
                }
                break;
            }
            case COMMAND_IDENT::RSL_STATE: {
                // Use the second byte to determine the rsl blink state
                mc.getModule<RSL>(RSL::MODULE_NAME)->setState(command[1]);
                // use the next 3 bytes to form an integer blink rate in Hz
                uint32_t blinkRateHz = (command[2] << 16) | (command[3] << 8) | command[4];
                if (blinkRateHz != 0) {
                    mc.getModule<RSL>(RSL::MODULE_NAME)->setBlink(blinkRateHz);
                }
                break;
            }
            case COMMAND_IDENT::ROBOT_DISABLE: {
                mc.getModule<RSL>(RSL::MODULE_NAME)->setState(RSLState::ON);
                break;
            }
            case COMMAND_IDENT::ROBOT_ENABLE: {
                mc.getModule<RSL>(RSL::MODULE_NAME)->setState(RSLState::BLINKING);
                break;
            }
            case COMMAND_IDENT::CONTROL_PINCHERS: {
                int position = command[1]; // Expecting position in degrees (0-180)
                Serial.print("Setting Pinchers to position: ");
                Serial.println(position);

                mc.getModule<Pinchers>(Pinchers::MODULE_NAME)->write(position);
                break;
            }
        }
    });
}

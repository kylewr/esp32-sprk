#include <Arduino.h>
#include <vector>

#include "ModuleCollection.hpp"
#include "SPISlaveWrapper.hpp"

#include "modules/Drivetrain.hpp"
#include "modules/LEDStatuses.hpp"
#include "modules/Pinchers.hpp"
#include "modules/StatusHandler.hpp"

#include <ESP32Servo.h>

#define LED 2

/*
These flags are the "build flags" that determine certain features of the build.
*/
#define USE_DEBUG_SERIAL

ModuleCollection mc;
SPISlaveWrapper sprkSPI;

StatusHandler* statusHandler = nullptr;

void setup() {
    Serial.begin(9600);

    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    mc.addModule(new LEDStatuses());
    mc.addModule(new Pinchers());
    mc.addModule(new Drivetrain());

    mc.initAll();

    statusHandler = new StatusHandler(&mc, &sprkSPI);

    sprkSPI.begin();

    Serial.println("Setup complete");
}

void loop() {
    mc.updateAll();

    sprkSPI.updateConnectionStatus();
    sprkSPI.listenAsync();

    statusHandler->updateMasterConnectionStatus(sprkSPI.isMasterConnected());

    sprkSPI.parseCommand([](byte* command) {
        using namespace SPIMappings;

#ifdef USE_DEBUG_SERIAL
        Serial.print("Command Received: ");
        for (size_t i = 0; i < SPIMappings::BUFFER_SIZE; i++) {
            Serial.print(command[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");
#endif

        COMMAND_IDENT ident = getCommandFromByte(command[0]);
        switch (ident) {
            case COMMAND_IDENT::NO_OP: {
                // Do nothing
                break;
            }
            case COMMAND_IDENT::MASTER_HEARTBEAT_DISABLE: {
                statusHandler->setHeartbeatType(COMMAND_IDENT::MASTER_HEARTBEAT_DISABLE);
                break;
            }
            case COMMAND_IDENT::MASTER_HEARTBEAT_ENABLED: {
                statusHandler->setHeartbeatType(COMMAND_IDENT::MASTER_HEARTBEAT_ENABLED);
                break;
            }
            case COMMAND_IDENT::SYSTEM_RESET: {
                statusHandler->disable();
                ESP.restart();
                break;
            }
            case COMMAND_IDENT::LEDS_ONBOARD_CTRL: {
                if (!StatusHandler::ALLOW_SPI_LED_CONTROL) {
                    break;
                }

                if (command[1] != 0x00) {
                    digitalWrite(LED, HIGH);
                } else {
                    digitalWrite(LED, LOW);
                }
                break;
            }
            case COMMAND_IDENT::LEDS_RSL_CTRL: {
                if (!StatusHandler::ALLOW_SPI_LED_CONTROL) {
                    break;
                }

                // Use the second byte to determine the rsl blink state
                mc.getModule<LEDStatuses>(LEDStatuses::MODULE_NAME)->setRSL(command[1]);
                // use the next 3 bytes to form an integer blink rate in Hz
                uint32_t blinkRateHz = (command[2] << 16) | (command[3] << 8) | command[4];
                if (blinkRateHz != 0) {
                    mc.getModule<LEDStatuses>(LEDStatuses::MODULE_NAME)->setRSLBlinkHz(blinkRateHz);
                }
                break;
            }
            case COMMAND_IDENT::ROBOT_DISABLE: {
                statusHandler->disable();
                mc.getModule<Drivetrain>(Drivetrain::MODULE_NAME)->setAllModules(false, false);
                break;
            }
            case COMMAND_IDENT::ROBOT_ENABLE: {
                statusHandler->enable();
                break;
            }
            case COMMAND_IDENT::CONTROL_DRIVETRAIN: {
                if (statusHandler->isEnabled()) {
                    mc.getModule<Drivetrain>(Drivetrain::MODULE_NAME)->accept(ident, command);   
                }
                break;
            }
            case COMMAND_IDENT::CONTROL_PINCHERS: {
                if (statusHandler->isEnabled()) {
                    mc.getModule<Pinchers>(Pinchers::MODULE_NAME)->accept(ident, command);
                }
                break;
            }
            case COMMAND_IDENT::TEST_ZERO: {
                digitalWrite(LED, LOW);
                statusHandler->invokeUnrecoverableError();
                // mc.getModule<Drivetrain>(Drivetrain::MODULE_NAME)->setAllModules(false, false);
                break;
            }
            case COMMAND_IDENT::TEST_ONE: {
                digitalWrite(LED, HIGH);
                // mc.getModule<Drivetrain>(Drivetrain::MODULE_NAME)->setAllModules(true, false);
                break;
            }
        }
    });
}

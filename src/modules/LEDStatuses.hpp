#pragma once

#include <Arduino.h>
#include <cstdint>

#include "Module.hpp"

#define RSL_PIN 21
#define STATUS_PIN 22

enum class LEDState { OFF, ON, BLINKING };
// enum class StatusDescriptions { NONE, NO_DS };

// inline constexpr uint32_t getStateNumber(LEDState state) {
//     switch (state) {
//         case LEDState::OFF:
//             return 0x00;
//         case LEDState::ON:
//             return 0x01;
//         case LEDState::BLINKING:
//             return 0x02;
//     }
//     return 0x00;
// }

class LEDStatuses : public Module {
    public:
        static constexpr const char* MODULE_NAME = "LEDS";

        LEDStatuses() : Module(MODULE_NAME) {}

        void init() override {
            pinMode(RSL_PIN, OUTPUT);
            digitalWrite(RSL_PIN, HIGH); // RSL on
            pinMode(STATUS_PIN, OUTPUT);
            digitalWrite(STATUS_PIN, LOW); // status off
        }

        void update() override {
            uint32_t currentTime = millis();
            if (state_RSL == LEDState::BLINKING) {
                if (currentTime - lastBlinkTime_RSL >= (1000 / hz_RSL)) {
                    blinkingIsOn_RSL = !blinkingIsOn_RSL;
                    digitalWrite(RSL_PIN, !blinkingIsOn_RSL);
                    lastBlinkTime_RSL = currentTime;
                }
            }
            if (state_STATUS == LEDState::BLINKING) {
                if (currentTime - lastBlinkTime_STATUS >= (1000 / hz_STATUS)) {
                    blinkingIsOn_STATUS = !blinkingIsOn_STATUS;
                    digitalWrite(STATUS_PIN, blinkingIsOn_STATUS);
                    lastBlinkTime_STATUS = currentTime;
                }
            }
        }

        void setRSL(uint8_t state) {
            switch (state) {
                case 0x01:
                    setRSL(LEDState::ON);
                    break;
                case 0x02:
                    setRSL(LEDState::BLINKING);
                    break;
                default:
                    setRSL(LEDState::OFF);
                    break;
            }
        }

        void setRSL(LEDState newState) {
            state_RSL = newState;
            switch (state_RSL) {
                case LEDState::OFF:
                    digitalWrite(RSL_PIN, LOW);
                    break;
                case LEDState::ON:
                    digitalWrite(RSL_PIN, HIGH);
                    break;
                case LEDState::BLINKING:
                    // Implement blinking logic if needed
                    break;
            }
        }

        void setStatus(uint8_t state) {
            switch (state) {
                case 0x01:
                    setStatus(LEDState::ON);
                    break;
                case 0x02:
                    setStatus(LEDState::BLINKING);
                    break;
                default:
                    setStatus(LEDState::OFF);
                    break;
            }
        }

        void setStatus(LEDState newState) {
            state_STATUS = newState;
            switch (newState) {
                case LEDState::OFF:
                    digitalWrite(STATUS_PIN, LOW);
                    break;
                case LEDState::ON:
                    digitalWrite(STATUS_PIN, HIGH);
                    break;
                case LEDState::BLINKING:
                    // Implement blinking logic if needed
                    break;
            }
        }

        void setBlink(uint32_t frequencyHz) {
            hz_RSL = frequencyHz;
        }

    private:

        uint32_t hz_RSL = 5;
        uint32_t hz_STATUS = 2;

        LEDState state_RSL = LEDState::ON;
        LEDState state_STATUS = LEDState::OFF;

        uint32_t lastBlinkTime_RSL = 0;
        uint32_t lastBlinkTime_STATUS = 0;

        bool blinkingIsOn_RSL = false;
        bool blinkingIsOn_STATUS = false;
};

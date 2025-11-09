#pragma once

#include <Arduino.h>
#include <cstdint>

#include "Module.hpp"

#define RSL_PIN 21

enum class RSLState { OFF, ON, BLINKING };

class RSL : public Module {
    public:
        static constexpr const char* MODULE_NAME = "RSL";

        RSL() : Module(MODULE_NAME) {}

        void init() override {
            pinMode(RSL_PIN, OUTPUT);
            digitalWrite(RSL_PIN, HIGH); // RSL on
        }

        void update() override {
            if (state == RSLState::BLINKING) {
                uint32_t currentTime = millis();
                if (currentTime - lastBlinkTime >= (1000 / hz)) {
                    blinkingIsOn = !blinkingIsOn;
                    digitalWrite(RSL_PIN, !blinkingIsOn);
                    lastBlinkTime = currentTime;
                }
            }
        }

        void setState(uint8_t state) {
            switch (state) {
                case 0x01:
                    setState(RSLState::ON);
                    break;
                case 0x02:
                    setState(RSLState::BLINKING);
                    break;
                default:
                    setState(RSLState::OFF);
                    break;
            }
        }

        void setState(RSLState newState) {
            state = newState;
            switch (state) {
                case RSLState::OFF:
                    digitalWrite(RSL_PIN, LOW);
                    break;
                case RSLState::ON:
                    digitalWrite(RSL_PIN, HIGH);
                    break;
                case RSLState::BLINKING:
                    // Implement blinking logic if needed
                    break;
            }
        }

        void setBlink(uint32_t frequencyHz) {
            hz = frequencyHz;
        }

    private:
        RSLState state = RSLState::OFF;
        uint32_t hz = 5; // default 5 Hz

        uint32_t lastBlinkTime = 0;
        bool blinkingIsOn = false;
};

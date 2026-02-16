#pragma once

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <cstdint>

#include "Module.hpp"

#define RSL_PIN 13
#define STATUS_PIN 48

#define STATUS_COLOR_R 32
#define STATUS_COLOR_G 8
#define STATUS_COLOR_B 0

enum class LEDState { OFF, ON, BLINKING };
// enum class LEDColor { RED, GREEN, BLUE };

class LEDStatuses : public Module {
    public:
        static constexpr const char* MODULE_NAME = "LEDS";

        LEDStatuses() : Module(MODULE_NAME) {}

        void init() override {
            pinMode(RSL_PIN, OUTPUT);
            digitalWrite(RSL_PIN, LOW);

            onboardLED.begin();
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
                    onboardLED.setPixelColor(0, blinkingIsOn_STATUS ? onboardLED.Color(STATUS_COLOR_R, STATUS_COLOR_G, STATUS_COLOR_B) : onboardLED.Color(0, 0, 0));
                    onboardLED.show();
                    lastBlinkTime_STATUS = currentTime;
                }
            }
        }

        void disableBehavior() override {}

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
                    lastBlinkTime_RSL = 0;
                    blinkingIsOn_RSL = true;
                    break;
            }
        }

        void setStatus(LEDState newState) {
            state_STATUS = newState;
            switch (newState) {
                case LEDState::OFF:
                    onboardLED.clear();
                    onboardLED.show();
                    break;
                case LEDState::ON:
                    onboardLED.setPixelColor(0, onboardLED.Color(STATUS_COLOR_R, STATUS_COLOR_G, STATUS_COLOR_B));
                    onboardLED.show();
                    break;
                case LEDState::BLINKING:
                    lastBlinkTime_STATUS = 0;
                    blinkingIsOn_STATUS = true;
                    break;
            }
        }

        void setRSLBlinkHz(uint32_t frequencyHz) {
            hz_RSL = frequencyHz;
        }

        void setStatusBlinkHz(uint32_t frequencyHz) {
            hz_STATUS = frequencyHz;
        }

    private:
        Adafruit_NeoPixel onboardLED = Adafruit_NeoPixel(1, STATUS_PIN, NEO_GRB + NEO_KHZ800);

        uint32_t hz_RSL = 5;
        uint32_t hz_STATUS = 2;

        LEDState state_RSL = LEDState::ON;
        LEDState state_STATUS = LEDState::OFF;

        uint32_t lastBlinkTime_RSL = 0;
        uint32_t lastBlinkTime_STATUS = 0;

        bool blinkingIsOn_RSL = false;
        bool blinkingIsOn_STATUS = false;
};

#pragma once

#include "modules/Module.hpp"
#include <ESP32PWM.h>

#define TEMP_HIGH 16
#define TEMP_LOW 17

class Drivetrain : public Module {
    public:
        static constexpr const char* MODULE_NAME = "DRIVETRAIN";

        Drivetrain() : Module(MODULE_NAME) {}

        void init() override {
            pinMode(TEMP_HIGH, OUTPUT);
            pinMode(TEMP_LOW, OUTPUT);

            digitalWrite(TEMP_HIGH, LOW);
            digitalWrite(TEMP_LOW, LOW);
        }

        void update() override {

        }

        void setDTState(bool low, bool high) {
            digitalWrite(TEMP_LOW, low ? HIGH : LOW);
            digitalWrite(TEMP_HIGH, high ? HIGH : LOW);
        }
    
};
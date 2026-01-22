#pragma once

#include "modules/Module.hpp"
#include <ESP32PWM.h>

#define FL_HIGH 26
#define FL_LOW 27
#define FR_HIGH 32
#define FR_LOW 33
#define BL_HIGH 13
#define BL_LOW 14
#define BR_HIGH 16
#define BR_LOW 17

class Drivetrain : public Module {
    public:
        static constexpr const char* MODULE_NAME = "DRIVETRAIN";

        Drivetrain() : Module(MODULE_NAME) {}

        void init() override {
            pinMode(FL_HIGH, OUTPUT);
            pinMode(FL_LOW, OUTPUT);
            pinMode(FR_HIGH, OUTPUT);
            pinMode(FR_LOW, OUTPUT);
            pinMode(BL_HIGH, OUTPUT);
            pinMode(BL_LOW, OUTPUT);
            pinMode(BR_HIGH, OUTPUT);
            pinMode(BR_LOW, OUTPUT);

            digitalWrite(FL_HIGH, LOW);
            digitalWrite(FL_LOW, LOW);
            digitalWrite(FR_HIGH, LOW);
            digitalWrite(FR_LOW, LOW);
            digitalWrite(BL_HIGH, LOW);
            digitalWrite(BL_LOW, LOW);
            digitalWrite(BR_HIGH, LOW);
            digitalWrite(BR_LOW, LOW);
        }

        void update() override {

        }

        void setDTState(bool low, bool high) {
            digitalWrite(FL_LOW, low ? HIGH : LOW);
            digitalWrite(FL_HIGH, high ? HIGH : LOW);
            digitalWrite(FR_LOW, low ? HIGH : LOW);
            digitalWrite(FR_HIGH, high ? HIGH : LOW);
            digitalWrite(BL_LOW, low ? HIGH : LOW);
            digitalWrite(BL_HIGH, high ? HIGH : LOW);
            digitalWrite(BR_LOW, low ? HIGH : LOW);
            digitalWrite(BR_HIGH, high ? HIGH : LOW);
        }
    
};
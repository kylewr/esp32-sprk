#pragma once

#include "modules/Module.hpp"
#include <ESP32PWM.h>

#define FL_HIGH 26
#define FL_LOW 27
#define FR_HIGH 32
#define FR_LOW 33
#define BL_HIGH 13
#define BL_LOW 25 // yes its weird but that way we dont conflict with the esp making the pin hgih at boot
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

        void update() override {}

        void disableBehavior() override {
            setAllModules(false, false);
        }

        void accept(COMMAND_IDENT ident, byte* data) override {
            switch (ident) {
                case COMMAND_IDENT::CONTROL_DRIVETRAIN: {
                    // convert unsigned 8 bit values for each module
                    int8_t fl = data[1] - 127;
                    int8_t fr = data[2] - 127;
                    int8_t bl = data[3] - 127;
                    int8_t br = data[4] - 127;

                    setModule(0, fl);
                    setModule(1, fr);
                    setModule(2, bl);
                    setModule(3, br);
                    break;
                }
                default: {
                    break;
                }
            }
        }

        void setAllModules(bool low, bool high) {
            digitalWrite(FL_LOW, low ? HIGH : LOW);
            digitalWrite(FL_HIGH, high ? HIGH : LOW);
            digitalWrite(FR_LOW, low ? HIGH : LOW);
            digitalWrite(FR_HIGH, high ? HIGH : LOW);
            digitalWrite(BL_LOW, low ? HIGH : LOW);
            digitalWrite(BL_HIGH, high ? HIGH : LOW);
            digitalWrite(BR_LOW, low ? HIGH : LOW);
            digitalWrite(BR_HIGH, high ? HIGH : LOW);
        }

        void setModule(uint8_t module, int vel) {
            if (vel == 0) {
                digitalWrite(MODULE_PINS[module][0], LOW);
                digitalWrite(MODULE_PINS[module][1], LOW);
                return;
            }

            digitalWrite(MODULE_PINS[module][0], vel < 0 ? HIGH : LOW);
            digitalWrite(MODULE_PINS[module][1], vel > 0 ? HIGH : LOW);
        }

    private:
        static constexpr int MODULE_PINS[4][2] = {
            {FL_LOW, FL_HIGH}, {FR_LOW, FR_HIGH}, {BL_LOW, BL_HIGH}, {BR_LOW, BR_HIGH}};
};

#pragma once

#include "Module.hpp"
#include <Arduino.h>
#include <cstdint>

#include <ESP32Servo.h>

#define PINCHERS_PIN 4

class Pinchers : public Module {
    public:
        static constexpr const char* MODULE_NAME = "PINCHERS";

        Pinchers() : Module(MODULE_NAME) {}

        void init() override {
            // pinMode(PINCHERS_PIN, OUTPUT);

            ESP32PWM::allocateTimer(1);

            servo.setPeriodHertz(50);

            servo.attach(PINCHERS_PIN, 0, 270); // Explicitly set min/max pulse widths
        }

        void update() override {
            // for (int i = 0; i <= 180; i++) {
            //     servo.write(i);
            //     delay(5);
            // }

            // for (int i = 180; i >= 0; i--) {
            //     servo.write(i);
            //     delay(5);
            // }
        }

        void write(int position) {
            if (position < 0)
                position = 0;
            if (position > 180)
                position = 180;
            servo.write(position);
            pos = position;
        }

    private:
        Servo servo;

        int pos {0};
};

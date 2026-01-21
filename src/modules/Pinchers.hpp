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
            ESP32PWM::allocateTimer(1);

            servo.setPeriodHertz(100);

            servo.attach(PINCHERS_PIN, 500, 2500); // microseconds
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
            if (position > 270)
                position = 270;
            
            position = map(position, 0, 270, 0, 180); // map to servo range
            servo.write(position);
            pos = position;
        }

        void writeMicroseconds(int pulse_us) {
            // Direct pulse width control (500-2500 μs)
            if (pulse_us < 500)
                pulse_us = 500;
            if (pulse_us > 2500)
                pulse_us = 2500;
            servo.writeMicroseconds(pulse_us);
        }

    private:
        Servo servo;

        int pos {0};
};

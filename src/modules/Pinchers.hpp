#pragma once

#include "Module.hpp"
#include <Arduino.h>
#include <cstdint>

#include <ESP32Servo.h>

#define PINCHERS_PIN 22

class Pinchers : public Module {
    public:
        static constexpr const char* MODULE_NAME = "PINCHERS";

        Pinchers() : Module(MODULE_NAME) {}

        void init() override {
            ESP32PWM::allocateTimer(0);

            servo.attach(PINCHERS_PIN);

        }

        void update() override {
            for (int i = 0; i <= 180; i++) {
                servo.write(i);
                delay(5);
            }

            for (int i = 180; i >= 0; i--) {
                servo.write(i);
                delay(5);
            }
        }
    
    private:
        Servo servo;

        int pos {0};
};

# esp32-sprk

## Parts of the project
1. The [SPRK robot program](https://github.com/kylewr/sprk) running on a Raspberry Pi.
2. The ESP-32 that runs custom software (this repository) that handles all low level IO on the robot and communicates to the Pi via SPI.
3. The [teleoperator console](https://github.com/kylewr/sprk-controller) that connects to the Pi via a network socket.

## Purpose
ESP32-SPRK was designed based on the FRC control system's RoboRIO. It connects to a computer (Pi in this case) running the robot program via SPI. It has many safety mechanisms to prevent the robot from running away or doing undefined behavior. The ESP controls PWM output for motors, servos, and stepper motors.

## Features
- Heartbeat to ensure connection
- Disabled/Enabled states shown via Robot Status Light (RSL)

## Building the project
This project uses PlatformIO with VSCode. It is the only environment I have tested it with. As long as you have the tools installed and have the correct Python version, the PlatfromIO build task should build the binary for ESP32.

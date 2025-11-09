#include <Arduino.h>
#include <vector>

#include "SPISlaveWrapper.hpp"

#define LED 2

// SPISlaveWrapper wrapper;

// static constexpr size_t BUFFER_SIZE = 16;
// uint8_t tx_buf[BUFFER_SIZE];
// uint8_t rx_buf[BUFFER_SIZE];

// ESP32SPISlave slave;

void setup() {
    Serial.begin(9600);
    delay(200);
    pinMode(LED, OUTPUT);

    // wrapper.begin();

    Serial.println("SPI Slave Initialized");
}

void loop() {
    // if (wrapper.hasNewCommand()) {
    //     Serial.println("New SPI Command Received");
    //     digitalWrite(LED, HIGH);
    // }


    // slave.queue(NULL, rx_buf, BUFFER_SIZE);

    // for (int i = 0; i < BUFFER_SIZE; i++) {
    //     tx_buf[i] = 0x44;
    // }

    // slave.queue(tx_buf, NULL, BUFFER_SIZE);

    // const std::vector<size_t> received_bytes = slave.wait();

    // Serial.print("Size: ");
    // Serial.print(received_bytes[0]);

    // Serial.print(". Data: ");
    // for (int i = 0; i < BUFFER_SIZE; i++) {
    //     Serial.print(rx_buf[i], HEX);
    //     Serial.print(" ");
    // }
    // Serial.println("");

    // memset(rx_buf, 0, BUFFER_SIZE);
}

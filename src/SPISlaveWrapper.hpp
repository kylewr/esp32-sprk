#pragma once

#include <vector>

#include "SPIMappings.hpp"
#include <ESP32SPISlave.h>

class SPISlaveWrapper {
    public:
        SPISlaveWrapper();
        ~SPISlaveWrapper() = default;

        bool begin();

        void listen();
        void listenAsync();

        bool hasNewCommand();

        uint8_t parseCommand();

        void queueSend(uint8_t* data);

        ESP32SPISlave& getSPI() {
            return spi;
        }

        uint8_t* getLatestCommand() {
            return latestCommand;
        }

    protected:
        ESP32SPISlave spi;

        uint8_t tx_buf[SPIMappings::BUFFER_SIZE];
        uint8_t rx_buf[SPIMappings::BUFFER_SIZE];

        uint8_t latestCommand[SPIMappings::BUFFER_SIZE];
};

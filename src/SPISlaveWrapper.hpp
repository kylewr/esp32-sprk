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

        bool checkAndClearBuffers();

        void parseCommand(std::function<void(byte*)> customHandler);

        void queueSend(uint8_t* data);
        void queueSend(const uint8_t* data);

        // Connection status methods
        bool isMasterConnected();
        void updateConnectionStatus();

        ESP32SPISlave& getSPI() {
            return spi;
        }

        uint8_t* getLatestCommand() {
            return latestCommand;
        }

    private:
        ESP32SPISlave spi;

        uint8_t tx_buf[SPIMappings::BUFFER_SIZE];
        uint8_t rx_buf[SPIMappings::BUFFER_SIZE];

        byte latestCommand[SPIMappings::BUFFER_SIZE];

        // Connection tracking
        unsigned long lastActivityTime;
        bool masterConnected;
        static constexpr unsigned long CONNECTION_TIMEOUT_MS = 1000; // 1 second
};

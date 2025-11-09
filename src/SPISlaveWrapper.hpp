#pragma once

#include <vector>

#include "SPIMappings.hpp"
#include <ESP32SPISlave.h>

class SPISlaveWrapper {
    public:
        SPISlaveWrapper();
        virtual ~SPISlaveWrapper() = default;

        virtual bool begin() final;

        virtual bool hasNewCommand();

        virtual void queueSend(uint8_t* data);

        virtual ESP32SPISlave& getSPI() final {
            return spi;
        }

    protected:
        ESP32SPISlave spi;

        uint8_t tx_buf[SPIMappings::BUFFER_SIZE];
        uint8_t rx_buf[SPIMappings::BUFFER_SIZE];
};

#include "SPISlaveWrapper.hpp"

#include <Arduino.h>

SPISlaveWrapper::SPISlaveWrapper() {
    spi.setDataMode(SPIMappings::SPI_MODE);

    memset(tx_buf, 0, SPIMappings::BUFFER_SIZE);
    memset(rx_buf, 0, SPIMappings::BUFFER_SIZE);
}

bool SPISlaveWrapper::begin() {
    return spi.begin(SPIMappings::SPI_BUS);
}

void SPISlaveWrapper::listen() {
    spi.queue(NULL, rx_buf, SPIMappings::BUFFER_SIZE);
    spi.queue(tx_buf, NULL, SPIMappings::BUFFER_SIZE);
    
    if (spi.wait().at(0) > 0) {
        memcpy(latestCommand, rx_buf, SPIMappings::BUFFER_SIZE);
    }
}

void SPISlaveWrapper::listenAsync() {
    memset(rx_buf, 0, SPIMappings::BUFFER_SIZE);

    if (spi.hasTransactionsCompletedAndAllResultsHandled()) {
        spi.queue(NULL, rx_buf, SPIMappings::BUFFER_SIZE);
        
        spi.queue(tx_buf, NULL, SPIMappings::BUFFER_SIZE);

        memset(tx_buf, 0, SPIMappings::BUFFER_SIZE);

        spi.trigger();
    }
}

bool SPISlaveWrapper::hasNewCommand() {
    if (spi.hasTransactionsCompletedAndAllResultsReady(1)) {
        memcpy(latestCommand, rx_buf, SPIMappings::BUFFER_SIZE);
        
        return spi.numBytesReceivedAll().at(0) > 0;
    }

    return false;
}

uint8_t SPISlaveWrapper::parseCommand() {

    return 0;
}

void SPISlaveWrapper::queueSend(uint8_t* data) {
    if (!spi.hasTransactionsCompletedAndAllResultsHandled()) {
        return;
    }

    // spi.queue(data, NULL, SPIMappings::Constants::BUFFER_SIZE);
}

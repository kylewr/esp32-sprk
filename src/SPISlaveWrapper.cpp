#include "SPISlaveWrapper.hpp"

#include <Arduino.h>
#include <functional>

using namespace SPIMappings;

SPISlaveWrapper::SPISlaveWrapper() {
    spi.setDataMode(SPI_MODE);

    memset(tx_buf, 0, BUFFER_SIZE);
    memset(rx_buf, 0, BUFFER_SIZE);
}

bool SPISlaveWrapper::begin() {
    return spi.begin(SPI_BUS);
}

void SPISlaveWrapper::listen() {
    spi.queue(NULL, rx_buf, BUFFER_SIZE);
    spi.queue(tx_buf, NULL, BUFFER_SIZE);

    if (spi.wait().at(0) > 0) {
        memcpy(latestCommand, rx_buf, BUFFER_SIZE);
    }
}

void SPISlaveWrapper::listenAsync() {
    if (spi.hasTransactionsCompletedAndAllResultsHandled()) {
        spi.queue(NULL, rx_buf, BUFFER_SIZE);

        spi.queue(tx_buf, NULL, BUFFER_SIZE);

        spi.trigger();
    }
}

bool SPISlaveWrapper::checkAndClearBuffers() {
    if (spi.hasTransactionsCompletedAndAllResultsReady(QUEUE_SIZE)) {
        memcpy(latestCommand, rx_buf, BUFFER_SIZE);
        memset(rx_buf, 0, BUFFER_SIZE);
        memset(tx_buf, 0, BUFFER_SIZE);

        return spi.numBytesReceivedAll().at(0) > 0;
    }

    return false;
}

void SPISlaveWrapper::parseCommand(std::function<void(byte*)> customHandler) {
    if (!checkAndClearBuffers()) {
        return;
    }

    if (customHandler) {
        customHandler(latestCommand);
    }

    memset(latestCommand, 0, BUFFER_SIZE);
}

void SPISlaveWrapper::queueSend(uint8_t* data) {
    if (!spi.hasTransactionsCompletedAndAllResultsHandled()) {
        return;
    }

    // spi.queue(data, NULL, Constants::BUFFER_SIZE);
}

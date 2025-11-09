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

bool SPISlaveWrapper::hasNewCommand() {
    if (spi.hasTransactionsCompletedAndAllResultsReady(SPIMappings::QUEUE_SIZE)) {
        std::vector<size_t> received_bytes = spi.numBytesReceivedAll();
        
        Serial.println("Start: ");
        for (int i = 0; i < received_bytes.size(); i++) {
            Serial.print(received_bytes[i]);
        }
        Serial.println(" End");
        
        return true;
    }

    return false;
}

void SPISlaveWrapper::queueSend(uint8_t* data) {
    if (!spi.hasTransactionsCompletedAndAllResultsHandled()) {
        return;
    }

    // spi.queue(data, NULL, SPIMappings::Constants::BUFFER_SIZE);
}

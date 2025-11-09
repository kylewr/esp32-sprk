#pragma once
#ifndef SPIMAPPINGS_HPP
#define SPIMAPPINGS_HPP

#include <cstddef>
#include <ESP32SPISlave.h>

namespace SPIMappings {
    static constexpr size_t BUFFER_SIZE = 16;
    static constexpr uint8_t QUEUE_SIZE = 4;
    static constexpr uint8_t SPI_BUS = VSPI;
    static constexpr uint8_t SPI_MODE = SPI_MODE0;

    enum class COMMAND {

    };
}; // namespace SPIMappings

#endif // SPIMAPPINGS_HPP

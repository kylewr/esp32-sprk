#pragma once
#ifndef SPIMAPPINGS_HPP
#define SPIMAPPINGS_HPP

#include <ESP32SPISlave.h>
#include <cstddef>
#include <unordered_map>

namespace SPIMappings {
    static constexpr size_t BUFFER_SIZE = 16;
    static constexpr uint8_t QUEUE_SIZE = 1;
    static constexpr uint8_t SPI_BUS = VSPI;
    static constexpr uint8_t SPI_MODE = SPI_MODE0;

    enum class COMMAND_IDENT : uint8_t {
        NOP = 0x00,

        // reserve 0x01-0x1F for system commands
        RESET = 0x10,

        // reserve 0x20-0x2F for LED statuses
        LED_CTRL = 0x20,
        RSL_STATE = 0x21,

        // reserve 0x30-0x3F for robot states
        ROBOT_DISABLE = 0x30,
        ROBOT_ENABLE = 0x31,

        // reserve 0x40-0xBF for user-defined commands
        CONTROL_DT = 0x40,
        CONTROL_PINCHERS = 0x51,
        
        // reserve 0xC0-CF for test commands
        TEST_ZERO = 0xC0,
        TEST_ONE = 0xC1,

    };

    static const std::unordered_map<uint8_t, COMMAND_IDENT> byteToIdent {
        {0x00, COMMAND_IDENT::NOP},
        {0x10, COMMAND_IDENT::RESET},
        {0x20, COMMAND_IDENT::LED_CTRL},
        {0x21, COMMAND_IDENT::RSL_STATE},
        {0x30, COMMAND_IDENT::ROBOT_DISABLE},
        {0x31, COMMAND_IDENT::ROBOT_ENABLE},
        {0x40, COMMAND_IDENT::CONTROL_DT},
        {0x51, COMMAND_IDENT::CONTROL_PINCHERS},
        {0xC0, COMMAND_IDENT::TEST_ZERO},
        {0xC1, COMMAND_IDENT::TEST_ONE},
    };

    inline COMMAND_IDENT identFromByte(uint8_t byte) {
        auto it = byteToIdent.find(byte);
        if (it != byteToIdent.end()) {
            return it->second;
        }
        return COMMAND_IDENT::NOP;
    };

}; // namespace SPIMappings

#endif // SPIMAPPINGS_HPP

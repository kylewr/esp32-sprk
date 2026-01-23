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
        // reserve 0x00-0x09 for no operation
        NO_OP = 0x00,
        MASTER_HEARTBEAT_DISABLE = 0x01,
        MASTER_HEARTBEAT_ENABLED = 0x02,

        // reserve 0x0A-0x1F for system commands
        SYSTEM_RESET = 0x10,

        // reserve 0x20-0x2F for LED statuses.
        LEDS_ONBOARD_CTRL = 0x20,
        LEDS_RSL_CTRL = 0x21,

        // reserve 0x30-0x3F for robot states
        ROBOT_DISABLE = 0x30,
        ROBOT_ENABLE = 0x31,

        // reserve 0x40-0xBF for user-defined commands
        CONTROL_DRIVETRAIN = 0x40,
        CONTROL_PINCHERS = 0x80,
        
        // reserve 0xC0-0xCF for test commands
        TEST_ZERO = 0xC0,
        TEST_ONE = 0xC1,
        
        // reserve 0xD0-0xFF for responses

    };

    static const std::unordered_map<uint8_t, COMMAND_IDENT> byteToCommand {
        {0x00, COMMAND_IDENT::NO_OP},
        {0x01, COMMAND_IDENT::MASTER_HEARTBEAT_DISABLE},
        {0x02, COMMAND_IDENT::MASTER_HEARTBEAT_ENABLED},
        {0x10, COMMAND_IDENT::SYSTEM_RESET},
        {0x20, COMMAND_IDENT::LEDS_ONBOARD_CTRL},
        {0x21, COMMAND_IDENT::LEDS_RSL_CTRL},
        {0x30, COMMAND_IDENT::ROBOT_DISABLE},
        {0x31, COMMAND_IDENT::ROBOT_ENABLE},
        {0x40, COMMAND_IDENT::CONTROL_DRIVETRAIN},
        {0x80, COMMAND_IDENT::CONTROL_PINCHERS},
        {0xC0, COMMAND_IDENT::TEST_ZERO},
        {0xC1, COMMAND_IDENT::TEST_ONE},
    };

    inline COMMAND_IDENT getCommandFromByte(uint8_t byte) {
        auto it = byteToCommand.find(byte);
        if (it != byteToCommand.end()) {
            return it->second;
        }
        return COMMAND_IDENT::NO_OP;
    };

    enum class RESPONSE_IDENT : uint8_t {
        ACK_NO_OP = 0x00, // Acknowledge No Operation

        ACK_ROBOT_DISABLE = 0xD0, // Acknowledge Robot Disable
        ACK_ROBOT_ENABLE = 0xD1,  // Acknowledge Robot Enable
    };

    static const std::unordered_map<uint8_t, RESPONSE_IDENT> byteToResponseIdent {
        {0x00, RESPONSE_IDENT::ACK_NO_OP},
        {0xD0, RESPONSE_IDENT::ACK_ROBOT_DISABLE},
        {0xD1, RESPONSE_IDENT::ACK_ROBOT_ENABLE},
    };

}; // namespace SPIMappings

#endif // SPIMAPPINGS_HPP

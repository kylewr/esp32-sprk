#pragma once

#include "ModuleCollection.hpp"
#include "SPIMappings.hpp"
#include "modules/LEDStatuses.hpp"

using namespace SPIMappings;

enum class StatusDescriptions { INIT, NO_MASTER, READY, UNRECOVERABLE_ERROR };

class StatusHandler {
    public:
        static constexpr const bool ALLOW_SPI_LED_CONTROL = false;

        StatusHandler(ModuleCollection* moduleCollection, SPISlaveWrapper* spiWrapper)
            : mc(moduleCollection), sprkSPI(spiWrapper) {
            /*
            The point of getting the ledstatuses and storing the pointer here
            is to avoid having to look it up from the MC every time.

            The LED statuses should be the source of truth for status indication.

            Unfortunately, we also need the SPISlave wrapper because on enable/disable
            we need to send ACKs back to the master.
            */

            if (mc == nullptr || sprkSPI == nullptr) {
                ledStatuses = nullptr;
                currentStatus = StatusDescriptions::UNRECOVERABLE_ERROR;
                return;
            }

            ledStatuses = mc->getModule<LEDStatuses>(LEDStatuses::MODULE_NAME);

            if (ledStatuses == nullptr) {
                currentStatus = StatusDescriptions::UNRECOVERABLE_ERROR;
            } else {
                setStatus(StatusDescriptions::NO_MASTER);
            }
        }

        inline bool isEnabled() const {
            return _isEnabled;
        }

        bool canEnable() const {
            return ledStatuses != nullptr &&
                   currentStatus != StatusDescriptions::UNRECOVERABLE_ERROR;
        }

        bool enable() {
            bool status = canEnable();

            if (!status) {
                return false;
            }

            ledStatuses->setRSL(LEDState::BLINKING);
            mc->enableAll();

            _isEnabled = true;

            // send an ack
            static const uint8_t ackEnable[SPIMappings::BUFFER_SIZE] = {
                static_cast<uint8_t>(RESPONSE_IDENT::ACK_ROBOT_ENABLE)};
            sprkSPI->queueSend(ackEnable);

            return true;
        }

        void disable() {
            _isEnabled = false;
            if (currentStatus != StatusDescriptions::UNRECOVERABLE_ERROR) {
                ledStatuses->setRSL(LEDState::ON);
            }
            mc->disableAll();

            // send an ack
            static const uint8_t ackDisable[SPIMappings::BUFFER_SIZE] = {
                static_cast<uint8_t>(RESPONSE_IDENT::ACK_ROBOT_DISABLE)};
            sprkSPI->queueSend(ackDisable);
        }

        inline StatusDescriptions getStatus() const {
            return currentStatus;
        }

        void setStatus(StatusDescriptions newStatus) {
            if (currentStatus == StatusDescriptions::UNRECOVERABLE_ERROR || newStatus == currentStatus) {
                return;
            }

            currentStatus = newStatus;

            if (ledStatuses == nullptr) {
                return;
            }

            switch (currentStatus) {
                case StatusDescriptions::INIT:
                    ledStatuses->setStatus(LEDState::OFF);
                    break;
                case StatusDescriptions::NO_MASTER:
                    if (isEnabled()) {
                        disable();
                    }
                    ledStatuses->setRSL(LEDState::OFF);

                    ledStatuses->setStatusBlinkHz(5);
                    ledStatuses->setStatus(LEDState::BLINKING);
                    break;
                case StatusDescriptions::READY:
                    ledStatuses->setStatus(LEDState::OFF);
                    break;
                case StatusDescriptions::UNRECOVERABLE_ERROR:
                    ledStatuses->setStatusBlinkHz(1);
                    ledStatuses->setStatus(LEDState::BLINKING);
                    ledStatuses->setRSL(LEDState::OFF);
                    break;
            }
        }

        void updateMasterConnectionStatus(bool hasMaster) {
            // // if we have the master connection but incorrect heartbeat, fix the state
            // if (hasMaster && isEnabled() && heartbeatType ==
            // COMMAND_IDENT::MASTER_HEARTBEAT_DISABLE) {
            //     disable();
            // } else if (hasMaster && !isEnabled() && heartbeatType ==
            // COMMAND_IDENT::MASTER_HEARTBEAT_ENABLED) {
            //     enable();
            // }

            // recovery checks
            if (hasMaster && !_hadMaster) {
                // just gained master connection
                if (!isEnabled() && heartbeatType == COMMAND_IDENT::MASTER_HEARTBEAT_ENABLED) {
                    enable();
                } else if (!isEnabled()) {
                    disable();
                }
                setStatus(StatusDescriptions::READY);
            } else if (!hasMaster && _hadMaster) {
                // just lost master connection
                disable();
                setStatus(StatusDescriptions::NO_MASTER);
            }

            _hadMaster = hasMaster;
        }

        void setHeartbeatType(COMMAND_IDENT heartbeatType) {
            if (heartbeatType == COMMAND_IDENT::MASTER_HEARTBEAT_DISABLE ||
                heartbeatType == COMMAND_IDENT::MASTER_HEARTBEAT_ENABLED) {
                this->heartbeatType = heartbeatType;
            }
        }

        void invokeUnrecoverableError() {
            setStatus(StatusDescriptions::UNRECOVERABLE_ERROR);
        }

        inline bool isInErrorState() const {
            return currentStatus == StatusDescriptions::UNRECOVERABLE_ERROR;
        }

    private:
        ModuleCollection* mc;
        SPISlaveWrapper* sprkSPI;
        LEDStatuses* ledStatuses;

        StatusDescriptions currentStatus = StatusDescriptions::INIT;
        COMMAND_IDENT heartbeatType = COMMAND_IDENT::MASTER_HEARTBEAT_DISABLE;

        bool _isEnabled = false;
        bool _hadMaster = false;
};

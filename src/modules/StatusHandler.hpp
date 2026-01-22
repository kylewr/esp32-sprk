#pragma once

#include "SPIMappings.hpp"
#include "ModuleCollection.hpp"
#include "modules/LEDStatuses.hpp"

using namespace SPIMappings;

enum class StatusDescriptions { INIT, NO_MASTER, READY, UNRECOVERABLE_ERROR };

class StatusHandler {
    public:
        static constexpr const bool ALLOW_SPI_LED_CONTROL = false;

        StatusHandler(ModuleCollection* moduleCollection) : mc(moduleCollection) {
            if (mc == nullptr) {
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

        bool isEnabled() const {
            return _isEnabled;
        }

        bool canEnable() const {
            return ledStatuses != nullptr && currentStatus != StatusDescriptions::UNRECOVERABLE_ERROR;
        }

        bool enable() {
            bool status = canEnable();

            if (!status) {
                return false;
            }

            ledStatuses->setRSL(LEDState::BLINKING);

            _isEnabled = true;
            return status;
        }

        void disable() {
            _isEnabled = false;
            ledStatuses->setRSL(LEDState::ON);
        }

        void setStatus(StatusDescriptions newStatus) {
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

    private:
        ModuleCollection* mc;
        LEDStatuses* ledStatuses;

        StatusDescriptions currentStatus = StatusDescriptions::INIT;
        COMMAND_IDENT heartbeatType = COMMAND_IDENT::MASTER_HEARTBEAT_DISABLE;

        bool _isEnabled = false;
        bool _hadMaster = false;
};

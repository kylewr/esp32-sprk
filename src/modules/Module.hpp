#pragma once

#include <string>

#include "SPIMappings.hpp"

using namespace SPIMappings;

class Module {
    public:
        Module(const std::string& name) : name(name) {}
        virtual ~Module() = default;

        virtual void init() = 0;
        virtual void update() = 0;
        virtual void disableBehavior() = 0; // we care more about the disable behavior, so make this pure virtual
        virtual void enableBehavior() {}; // optional to implement

        virtual void accept(COMMAND_IDENT ident, byte* data) {}

        const std::string& getName() const {
            return name;
        }

    private:
        const std::string name;
};
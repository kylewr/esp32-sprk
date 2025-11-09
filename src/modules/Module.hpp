#pragma once

#include <string>

class Module {
    public:
        Module(const std::string& name) : name(name) {}
        virtual ~Module() = default;

        virtual void init() = 0;
        virtual void update() = 0;

        const std::string& getName() const {
            return name;
        }

    private:
        const std::string name;
};
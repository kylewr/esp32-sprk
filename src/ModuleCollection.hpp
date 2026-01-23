#pragma once

#ifndef MODULECOLLECTION_HPP
#define MODULECOLLECTION_HPP

#include "modules/Module.hpp"
#include <string>
#include <unordered_map>

class ModuleCollection {
    public:
        ModuleCollection() = default;
        ~ModuleCollection() {
            for (auto& [name, module] : modules) {
                delete module;
            }
        };

        void addModule(Module* module) {
            for (auto& [name, mod] : modules) {
                if (mod == module) {
                    return; // Module already exists
                }
            }

            modules[module->getName()] = module;
        }

        void initAll() {
            for (auto& [name, module] : modules) {
                module->init();
            }
        }

        void updateAll() {
            for (auto& [name, module] : modules) {
                module->update();
            }
        }

        void disableAll() {
            for (auto& [name, module] : modules) {
                module->disableBehavior();
            }
        }
        
        void enableAll() {
            for (auto& [name, module] : modules) {
                module->enableBehavior();
            }
        }

        Module* getModule(const std::string& name) {
            auto it = modules.find(name);
            if (it != modules.end()) {
                return it->second;
            }
            return nullptr;
        }

        template <typename T> T* getModule(const std::string& name) {
            auto mod = getModule(name);
            if (mod) {
                return static_cast<T*>(mod);
            }
            return nullptr;
        }

        template <typename T> T* getModule() {
            for (auto& [name, module] : modules) {
                T* casted = static_cast<T*>(module);
                if (casted) {
                    return casted;
                }
            }
            return nullptr;
        }

    private:
        std::unordered_map<std::string, Module*> modules;
};

#endif

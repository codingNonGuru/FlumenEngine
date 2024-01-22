#pragma once

#include "FlumenCore/Singleton.h"
#include "FlumenCore/Conventions.hpp"

namespace engine
{
    class ConfigManager : public core::Singleton <ConfigManager>
    {
        friend class core::Singleton <ConfigManager>;

        union ConfigValue
        {
            Word String;

            int Integer;

            float Float;

            ConfigValue() {}

            ConfigValue(Word string) : String(string) {}

            ConfigValue(int integerValue) : Integer(integerValue) {}

            ConfigValue(float floatValue) : Float(floatValue) {}
        };

        ConfigManager();

    public:
        ConfigValue GetValue(Word);
    };
}
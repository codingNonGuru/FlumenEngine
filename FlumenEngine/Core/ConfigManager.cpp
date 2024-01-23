#include <nlohmann/json.hpp>

#include "ConfigManager.h"
#include "FlumenEngine/Core/FileManager.hpp"
#include "FlumenEngine/Core/File.hpp"

using namespace engine;
using json = nlohmann::json;

static const auto CONFIG_FILE_NAME = "Config.json";

#define CONFIG_NOT_FOUND_MESSAGE "Fatal error: Engine can not find Config.json!\n"

nlohmann::json_abi_v3_11_3::basic_json<> configObject;

ConfigManager::ConfigManager()
{
    auto file = engine::FileManager::Get()->GetFile(CONFIG_FILE_NAME);

    assert((file != nullptr) && (CONFIG_NOT_FOUND_MESSAGE));

    auto fileContent = file->ReadContent();

    configObject = json::parse(fileContent);
}

ConfigManager::ConfigValue ConfigManager::GetValue(LongWord valueName)
{
    auto &value = configObject.at(valueName.Get());
    if(value.is_number_float() == true)
    {
        auto result = ConfigValue();

        value.get_to(result.Float);

        return result;
    }
    else if(value.is_number_integer() == true)
    {
        auto result = ConfigValue();

        value.get_to(result.Integer);

        return result;
    }
    else if(value.is_string() == true)
    {
        std::string stringValue;
        value.get_to(stringValue);

        auto result = ConfigValue(stringValue.c_str());

        return result;
    }
}
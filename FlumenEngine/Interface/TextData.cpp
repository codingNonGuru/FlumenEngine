#include "TextData.h"
#include "FlumenEngine/Core/ConfigManager.h"
#include "FlumenEngine/Config.h"

FontDescriptor::FontDescriptor() {}

FontDescriptor::FontDescriptor(Word size) 
{
    static auto fontType = engine::ConfigManager::Get()->GetValue(engine::ConfigValues::DEFAULT_INTERFACE_FONT).String;

    Name << fontType << size;
}

FontDescriptor::FontDescriptor(Word name, Word size) 
{
    Name << name << size;
}
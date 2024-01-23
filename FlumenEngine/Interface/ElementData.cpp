#include "ElementData.h"
#include "FlumenEngine/Core/ConfigManager.h"
#include "FlumenEngine/Config.h"

SpriteDescriptor::SpriteDescriptor(const char *textureName, bool isSliced) : HasTexture(true), isInitialized(true), TextureName(textureName)
{
    std::cout<<textureName<<"\n";
    if(isSliced == true)
    {
        static auto slicedShaderName = engine::ConfigManager::Get()->GetValue(engine::ConfigValues::DEFAULT_INTERFACE_9_SLICE_SHADER).String;
        ShaderName = slicedShaderName; 
    }
    else
    {
        static auto basicShaderName = engine::ConfigManager::Get()->GetValue(engine::ConfigValues::DEFAULT_INTERFACE_SHADER).String;
        ShaderName = basicShaderName; 
    }
}

SpriteDescriptor::SpriteDescriptor(Word textureName, bool isSliced) : HasTexture(true), isInitialized(true), TextureName(textureName)
{
    std::cout<<textureName<<"\n";
    if(isSliced == true)
    {
        static auto slicedShaderName = engine::ConfigManager::Get()->GetValue(engine::ConfigValues::DEFAULT_INTERFACE_9_SLICE_SHADER).String;
        ShaderName = slicedShaderName; 
    }
    else
    {
        static auto basicShaderName = engine::ConfigManager::Get()->GetValue(engine::ConfigValues::DEFAULT_INTERFACE_SHADER).String;
        ShaderName = basicShaderName; 
    }
}
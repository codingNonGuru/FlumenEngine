#include <SDL2/SDL_mixer.h>

#include "FlumenEngine/Sound/SoundManager.h"
#include "FlumenEngine/Sound/Sound.h"
#include "FlumenEngine/Core/AssetManager.hpp"
#include "FlumenEngine/Core/File.hpp"
#include "FlumenEngine/Config.h"

#define MAXIMUM_SIMULTANEOUS_SOUNDS 8

#define MAXIMUM_LOADED_SOUNDS 128

namespace engine
{
    void SoundManager::LoadSounds()
    {
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MAXIMUM_SIMULTANEOUS_SOUNDS, 2048);

        sounds.Initialize(MAXIMUM_LOADED_SOUNDS);

	    auto files = AssetManager::GetFiles();

        for(auto &file : *files)
        {
            auto extension = ".wav";

            auto extensionPosition = FindString(file.GetName(), extension);
            if(extensionPosition == nullptr)
                continue;

            Word soundName;
            soundName.Add(file.GetName(), extensionPosition - file.GetName());

            auto sound = sounds.Add(soundName);
            if(sound == nullptr)
                continue;

            sound->Initialize(&file);
        }

        static const auto DEFAULT_SOUND_VOLUME = engine::ConfigManager::Get()->GetValue(engine::ConfigValues::DEFAULT_SOUND_VOLUME).Float;
        SetVolume(DEFAULT_SOUND_VOLUME);
    }

    void SoundManager::PlaySound(Word soundName)
    {
        auto sound = sounds.Get(soundName);

        sound->Play();
    }

    void SoundManager::SetVolume(float volume)
    {
        Mix_Volume(-1, int(volume * (float)MIX_MAX_VOLUME));
    }
}
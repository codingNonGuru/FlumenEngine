#include <SDL2/SDL_mixer.h>

#include "FlumenEngine/Sound/SoundManager.h"
#include "FlumenEngine/Sound/Sound.h"
#include "FlumenEngine/Core/AssetManager.hpp"
#include "FlumenEngine/Core/File.hpp"

namespace engine
{
    void SoundManager::LoadSounds()
    {
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

        sounds.Initialize(32);

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
    }

    void SoundManager::PlaySound(Word soundName)
    {
        auto sound = sounds.Get(soundName);

        sound->Play();
    }
}
#include <SDL2/SDL_mixer.h>

#include "FlumenEngine/Sound/Sound.h"
#include "FlumenEngine/Core/File.hpp"

namespace engine
{
    void Sound::Initialize(File *file)
    {
        chunk = Mix_LoadWAV(file->GetPath());
    }

    void Sound::Play()
    {
        Mix_PlayChannel(-1, chunk, 0);
    }
}
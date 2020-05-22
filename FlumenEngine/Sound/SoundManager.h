#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Core/Singleton.h"

namespace engine
{
    class Sound;

    class SoundManager : public Singleton<SoundManager>
    {
    private:
        Map <Sound> sounds;

    public:
        SoundManager() {}

        void LoadSounds();

        void PlaySound(Word);
    };
}
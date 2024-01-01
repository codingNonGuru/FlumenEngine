#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenCore/Singleton.h"

namespace engine
{
    class Sound;

    class SoundManager : public core::Singleton <SoundManager>
    {
    private:
        Map <Sound> sounds;

    public:
        SoundManager() {}

        void LoadSounds();

        void PlaySound(Word);
    };
}
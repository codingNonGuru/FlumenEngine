#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenCore/Singleton.h"

namespace engine
{
    class Sound;

    class SoundManager : public core::Singleton <SoundManager>
    {
        friend class core::Singleton <SoundManager>;
    
        Map <Sound> sounds;

        SoundManager() {}

    public:
        void LoadSounds();

        void PlaySound(Word);

        void SetVolume(float);
    };
}
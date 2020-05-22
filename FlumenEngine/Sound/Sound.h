#pragma once

struct Mix_Chunk;

class File;

namespace engine
{
    class Sound
    {
    private:
        Mix_Chunk *chunk;

    public:
        Sound() {}

        void Initialize(File *);

        void Play();
    };
}
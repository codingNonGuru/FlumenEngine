#pragma once

#include "FlumenEngine/Core/ConfigManager.h"

namespace engine
{
    #define THREAD_COUNT 4

    #define MAXIMUM_ASYNC_TASK_COUNT 16

    struct ConfigValues
    {
        static constexpr auto DEFAULT_OPENGL_MAJOR_VERSION = "DEFAULT_OPENGL_MAJOR_VERSION";
        static constexpr auto DEFAULT_OPENGL_MINOR_VERSION = "DEFAULT_OPENGL_MINOR_VERSION";
        static constexpr auto DEFAULT_SCREEN_WIDTH = "DEFAULT_SCREEN_WIDTH";
        static constexpr auto DEFAULT_SCREEN_HEIGHT = "DEFAULT_SCREEN_HEIGHT";
        static constexpr auto DEFAULT_INTERFACE_SHADER = "DEFAULT_INTERFACE_SHADER";
        static constexpr auto DEFAULT_INTERFACE_9_SLICE_SHADER = "DEFAULT_INTERFACE_9_SLICE_SHADER";
        static constexpr auto DEFAULT_INTERFACE_FONT = "DEFAULT_INTERFACE_FONT";  
        static constexpr auto DEFAULT_SOUND_VOLUME = "DEFAULT_SOUND_VOLUME";  
    };
}
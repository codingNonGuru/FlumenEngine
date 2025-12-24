#pragma once

#include "FlumenCore/Conventions.hpp"

class Camera;

namespace engine::render
{
    class HexRenderer
    {
    public:
        static void RenderHex(Camera *, Position2, float, float, Float4);

        static void RenderEmptyHex(Camera *, Position2, float, float, Float4, float);
    };
}
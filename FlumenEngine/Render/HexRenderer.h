#pragma once

#include "FlumenCore/Conventions.hpp"

class Camera;

namespace engine::render
{
    class HexRenderer
    {
    public:
        static void RenderHex(Camera *, Position2, float, float, Float4);
    };
}
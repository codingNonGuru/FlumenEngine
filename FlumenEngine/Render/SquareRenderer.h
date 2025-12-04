#pragma once

#include "FlumenCore/Conventions.hpp"

class Camera;

namespace engine::render
{
    class SquareRenderer
    {
    public:
        static void DrawSquare(Camera *, Position2, Scale2, float, Float4, float, float = 0.0f);
    };
}
#pragma once

#include "FlumenCore/Conventions.hpp"

class Mesh;
class Camera;

class PickHandler
{
public:
    static bool CheckCollision(Camera *, Mesh *, Float3, Float = 1.0f);
};

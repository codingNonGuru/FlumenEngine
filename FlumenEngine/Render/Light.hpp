#pragma once

#include "FlumenCore/Conventions.hpp"

class Light
{
	Float3 direction_;

	Float3 position_;

	Float3 color_;

	Float coneWidth_;

public:
	Light();

	Light(Float3);

	Float3 GetDirection() const;

	Matrix GetShadowMatrix(Float, Float3) const;
};

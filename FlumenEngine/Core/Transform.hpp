#pragma once

#include <glm/glm.hpp>

#include "FlumenCore/Conventions.hpp"

class Transform
{
private:
	Position3 position_;

	Rotation rotation_;

	Scale scale_;

public:
	Transform();

	Transform(Position2);

	Transform(Position2, Scale);

	Transform(Position3);

	Transform(Position3, Rotation, Scale);

	void Initialize(Position3, Rotation, Scale);

	Position3 & GetPosition();

	Rotation & GetRotation();

	Scale & GetScale();

	Direction3 GetForward();

	Matrix GetMatrix();

	Matrix GetPositionMatrix();

	//static Transform* Allocate(Scene*, Position, Rotation, Scale);
	//static Transform* Allocate(Scene*, Transform*);
	virtual ~Transform();
};

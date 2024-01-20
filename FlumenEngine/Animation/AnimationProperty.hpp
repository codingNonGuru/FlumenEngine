#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenEngine/Core/Types.hpp"

class Animation;
class AnimationKey;

class AnimationProperty
{
private:
	Animation * animation_;

	AnimationValueTypes valueType_;

	const float *targetValue_;

	Array <AnimationKey> keys_;

public:
	AnimationProperty();

	AnimationProperty(Animation*);

	AnimationProperty(Animation*, const float *);

	void Initialize(const float *);

	AnimationKey* AddKey();

	void Update();
};


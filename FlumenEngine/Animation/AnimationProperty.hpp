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

	void * targetValue_;

	Array <AnimationKey> keys_;

public:
	AnimationProperty();

	AnimationProperty(Animation*);

	AnimationProperty(Animation*, float*);

	void Initialize(float*);

	AnimationKey* AddKey();

	void Update();
};


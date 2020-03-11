#include "FlumenEngine/Animation/AnimationProperty.hpp"

#include "FlumenEngine/Animation/Animation.hpp"
#include "AnimationKey.hpp"

AnimationProperty::AnimationProperty()
{
	valueType_ = AnimationValueTypes::NONE;

	keys_.Initialize(8);
}

AnimationProperty::AnimationProperty(Animation* animation, float* targetValue)
{
	animation_ = animation;

	keys_.Initialize(8);

	Initialize(targetValue);
}

AnimationProperty::AnimationProperty(Animation* animation)
{
	animation_ = animation;

	keys_.Initialize(8);
}

void AnimationProperty::Initialize(float* targetValue)
{
	valueType_ = AnimationValueTypes::FLOAT;

	targetValue_ = targetValue;
}

AnimationKey* AnimationProperty::AddKey()
{
	auto keyPointer = keys_.Allocate();

	return keyPointer;
}

float Fade(float t)
{
	return 3.0f * t * t - 2.0f * t * t * t;
}

void AnimationProperty::Update()
{
	if(!animation_)
		return;

	float time = animation_->GetTime();

	AnimationKey* firstKey = nullptr;
	AnimationKey* secondKey = nullptr;

	for(auto key = keys_.GetStart(); key != keys_.GetEnd(); ++key)
	{
		if(firstKey)
		{
			if(key->GetFrame() > firstKey->GetFrame() && key->GetFrame() <= time)
				firstKey = key;
		}
		else
		{
			if(key->GetFrame() <= time)
				firstKey = key;
		}
	}

	secondKey = keys_.GetNext(firstKey);

	if(!secondKey && firstKey)
	{
		secondKey = firstKey;
	}

	if(!firstKey && !secondKey)
		return;

	float length = secondKey->GetFrame() - firstKey->GetFrame();

	if(length == 0.0f)
	{
		time = 1.0f;
	}
	else
	{
		time = (time - firstKey->GetFrame()) / length;
	}

	switch(valueType_)
	{
	case AnimationValueTypes::FLOAT:
		float start = *((float*)firstKey->GetValue());
		float end = *((float*)secondKey->GetValue());
		*((float*)targetValue_) = (1.0f - Fade(time)) * start + Fade(time) * end;
		break;
	}
}

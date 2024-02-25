#include "FlumenEngine/Animation/AnimationProperty.hpp"

#include "FlumenEngine/Animation/Animation.hpp"
#include "AnimationKey.hpp"

#define MAXIMUM_KEY_COUNT 8

AnimationProperty::AnimationProperty()
{
	valueType_ = AnimationValueTypes::NONE;

	keys_.Initialize(MAXIMUM_KEY_COUNT);
}

AnimationProperty::AnimationProperty(Animation* animation, const float *targetValue)
{
	animation_ = animation;

	keys_.Initialize(MAXIMUM_KEY_COUNT);

	Initialize(targetValue);
}

AnimationProperty::AnimationProperty(Animation* animation)
{
	animation_ = animation;

	keys_.Initialize(8);
}

void AnimationProperty::Initialize(const float* targetValue)
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
		const float start = *((float*)firstKey->GetValue());
		const float end = *((float*)secondKey->GetValue());

		const auto processedTime = Fade(time);

		*((float*)targetValue_) = (1.0f - processedTime) * start + processedTime * end;
		break;
	}
}

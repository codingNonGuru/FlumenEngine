#include "AnimationKey.hpp"

AnimationKey::AnimationKey()
{
	value_ = nullptr;
}

void * AnimationKey::GetValue()
{
	return value_;
}

float AnimationKey::GetFrame()
{
	return frame_;
}

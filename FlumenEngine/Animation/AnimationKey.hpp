#pragma once

class AnimationKey
{
	float frame_;

	void * value_;

public:
	AnimationKey();

	template <typename ValueType>
	void Initialize(float, ValueType);

	void * GetValue();

	float GetFrame();
};

template <typename ValueType>
void AnimationKey::Initialize(float frame, ValueType value)
{
	frame_ = frame;

	value_ = AllocateMemory(sizeof(ValueType));
	CopyMemory(value_, &value, sizeof(ValueType));
}

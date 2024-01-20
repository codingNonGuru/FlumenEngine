#include "FlumenEngine/Animation/Animation.hpp"

#include "FlumenEngine/Animation/AnimationProperty.hpp"
#include "FlumenEngine/Animation/AnimationEvent.hpp"
#include "FlumenCore/Time.hpp"

Animation::Animation()
{
	isPlaying_ = false;

	length_ = 1.0f;

	properties_.Initialize(8);

	events_.Initialize(8);
}

Animation::Animation(float length)
{
	isPlaying_ = false;

	length_ = length;

	properties_.Initialize(8);

	events_.Initialize(8);

	startEvent_ = AddEvent();
	startEvent_->Initialize(0.0f);

	finishEvent_ = AddEvent();
	finishEvent_->Initialize(length);
}

AnimationProperty* Animation::AddProperty()
{
	auto propertyPointer = properties_.Allocate();
	if(!propertyPointer)
		return nullptr;

	*propertyPointer = AnimationProperty(this);

	return propertyPointer;
}

AnimationProperty *Animation::AddProperty(AnimationProperty property)
{
	auto propertyPointer = properties_.Allocate();
	if(!propertyPointer)
		return;

	*propertyPointer = property;

	return propertyPointer;
}

AnimationEvent* Animation::AddEvent()
{
	auto event = events_.Allocate();

	return event;
}

void Animation::Play()
{
	isPlaying_ = true;

	willStop_ = false;

	time_ = 0.0f;

	for(auto event = events_.GetStart(); event != events_.GetEnd(); ++event)
	{
		event->Reset();
	}
}

void Animation::Update()
{
	if(!isPlaying_)
		return;

	for(auto property = properties_.GetStart(); property != properties_.GetEnd(); ++property)
	{
		property->Update();
	}

	for(auto event = events_.GetStart(); event != events_.GetEnd(); ++event)
	{
		if(event->IsInvoked())
			continue;

		if(time_ < event->GetTime())
			continue;

		event->Invoke();
	}

	if(willStop_)
	{
		isPlaying_ = false;
		return;
	}

	time_ += Time::GetDelta();

	if(time_ > length_)
	{
		time_ = length_;
		willStop_ = true;
	}
}

float Animation::GetTime()
{
	return time_;
}

float Animation::GetLength()
{
	return length_;
}

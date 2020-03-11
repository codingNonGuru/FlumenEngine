#pragma once

#include "FlumenCore/Conventions.hpp"

class AnimationProperty;
class AnimationEvent;

class Animation
{
private:
	bool isPlaying_;

	bool willStop_;

	bool isLooping_;

	float time_;

	float length_;

	Array <AnimationProperty> properties_;

	Array <AnimationEvent> events_;

	AnimationEvent* startEvent_;

	AnimationEvent* finishEvent_;

public:
	Animation();

	Animation(float);

	void Play();

	void Update();

	AnimationProperty* AddProperty();

	AnimationEvent* AddEvent();

	void AddProperty(AnimationProperty);

	AnimationEvent* GetStartEvent() {return startEvent_;}

	AnimationEvent* GetFinishEvent() {return finishEvent_;}

	float GetTime();

	float GetLength();
};

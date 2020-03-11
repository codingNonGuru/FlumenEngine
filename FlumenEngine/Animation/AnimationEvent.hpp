#pragma once

class Delegate;

class AnimationEvent
{
private:
	float time_;

	bool isInvoked_;

	Delegate * actions_;

public:
	AnimationEvent();

	AnimationEvent(float);

	void Initialize(float);

	Delegate & GetActions();

	float GetTime();

	void Invoke();

	void Reset();

	bool IsInvoked();
};

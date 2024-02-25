#include "FlumenEngine/Animation/AnimationEvent.hpp"

#include "FlumenCore/Delegate/Delegate.hpp"

AnimationEvent::AnimationEvent()
{
	actions_ = nullptr;
}

AnimationEvent::AnimationEvent(float time) : actions_(nullptr)
{
	Initialize(time);
}

void AnimationEvent::Initialize(float time)
{
	time_ = time;

	if(actions_ == nullptr)
	{
		actions_ = new Delegate();
	}
}

float AnimationEvent::GetTime()
{
	return time_;
}

Delegate & AnimationEvent::GetActions()
{
	return *actions_;
}

void AnimationEvent::Invoke()
{
	if(isInvoked_)
		return;

	actions_->Invoke();

	isInvoked_ = true;
}

void AnimationEvent::Reset()
{
	isInvoked_ = false;
}

bool AnimationEvent::IsInvoked()
{
	return isInvoked_;
}

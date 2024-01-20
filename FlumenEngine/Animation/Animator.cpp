#include "FlumenEngine/Animation/Animator.hpp"

#include "FlumenEngine/Animation/Animation.hpp"

Animator::Animator()
{
	animations_.Initialize(8);
}

Animation* Animator::GetAnimation(const char* name)
{
	auto animation = animations_.Get(name);
	if(animation == nullptr)
	{
		return nullptr;
	}

	return *animation;
}

Animation* Animator::AddAnimation(Animation* animation, const char* name)
{
	auto animationPointer = animations_.Add(name);
	if(!animationPointer)
		return nullptr;

	*animationPointer = animation;

	return *animationPointer;
}

void Animator::Update()
{
	for(auto animationIterator = animations_.GetStart(); animationIterator != animations_.GetEnd(); ++animationIterator)
	{
		auto animation = *animationIterator;
		if(!animation)
			return;

		animation->Update();
	}
}

void Animator::Play(const char* name)
{
	auto animation = animations_.Get(name);

	if(!animation)
		return;

	(*animation)->Play();
}


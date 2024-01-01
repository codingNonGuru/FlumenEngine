#include "FlumenEngine/Render/Filter.hpp"

#include "FlumenCore/Conventions.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Render/Texture.hpp"
#include "FlumenEngine/Render/Camera.hpp"
#include "FlumenCore/Time.hpp"

Filter::Filter() {}

Filter::Filter(Shader* shader, render::Texture* screenTexture)
{
	Initialize(shader, screenTexture);
}

void Filter::Initialize(Shader* shader, render::Texture* screenTexture)
{
	isActive_ = false;

	timer_ = 0.0f;

	transition_ = Transitions::NONE;

	shader_ = shader;

	screenTexture_ = screenTexture;

	HandleInitialize();
}

void Filter::Open()
{
	isActive_ = true;

	transition_ = Transitions::OPEN;

	timer_ = 0.0f;
}

void Filter::Close()
{
	transition_ = Transitions::CLOSE;

	timer_ = 1.0f;
}

void Filter::Update(Camera* camera)
{
	if(!isActive_)
		return;

	float transitionSpeed = 2.0f;
	if(transition_ == Transitions::OPEN)
	{
		timer_ += Time::GetDelta() * transitionSpeed;
		if(timer_ > 1.0f)
		{
			timer_ = 1.0f;
			transition_ = Transitions::NONE;
		}
	}
	else if(transition_ == Transitions::CLOSE)
	{
		timer_ -= Time::GetDelta() * transitionSpeed;
		if(timer_ < 0.0f)
		{
			timer_ = 0.0f;
			transition_ = Transitions::NONE;

			isActive_ = false;
		}
	}

	if(!screenTexture_)
		return;

	screenTexture_->DownloadBackBuffer();

	HandleUpdate(camera);
}

float Filter::GetTimeFactor()
{
	return 3.0f * timer_ * timer_ - 2.0f * timer_ * timer_ * timer_;
}

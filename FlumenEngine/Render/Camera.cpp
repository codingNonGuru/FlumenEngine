#include <stdlib.h>

#include <glm/gtx/transform.hpp>

#include "FlumenEngine/Render/Camera.hpp"
#include "FlumenEngine/Render/Screen.hpp"

Camera::Camera() {}

Camera::Camera(Screen* screen, Position3 to, float zenith, float azimuth, float viewDistance)
{
	screen_ = screen;

	to_ = to;

	zenith_ = zenith;

	azimuth_ = azimuth;

	viewDistance_ = viewDistance;

	viewDirection_ = glm::vec3(sin(zenith_) * cos(azimuth_), sin(zenith_) * sin(azimuth_), cos(zenith_));

	from_ = to_ + (viewDirection_ * viewDistance_);

	up_ = Direction3(0.0f, 0.0f, 1.0f);

	scrollImpulse_ = 0.0f;
	spinImpulse_ = 0.0f;
	driftImpulse_ = Direction3(0.0f, 0.0f, 0.0f);

	type_ = CameraTypes::PERSPECTIVE;
}

Camera::Camera(Screen* screen, Position3 from, Position3 to)
{
	screen_ = screen;

	from_ = from;

	to_ = to;

	up_ = Direction3(0.0f, 0.0f, 1.0f);

	pivot_ = to_;

	type_ = CameraTypes::PERSPECTIVE;

	spinImpulse_ = 0.0f;
}

Camera::Camera(Screen* screen)
{
	screen_ = screen;

	type_ = CameraTypes::ORTHO;
}

Matrix & Camera::GetMatrix()
{
	return finalMatrix_;
}

void Camera::ComputeMatrix()
{
	if(type_ == CameraTypes::ORTHO)
	{
		glm::vec3 screenCenter(-screen_->getWidthFloating() * 0.5f, -screen_->getHeightFloating() * 0.5f, 0.0f);

		glm::mat4 projectionMatrix = glm::ortho<float> (0.0f, screen_->getWidthFloating(), screen_->getHeightFloating(), 0.0f, 0.1f, 10.0f);

		glm::mat4 viewMatrix = glm::lookAt<float> (screenCenter + glm::vec3(0.0f, 0.0f, 1.0f), screenCenter, glm::vec3(0.0f, 1.0f, 0.0f));

		//viewMatrix = glm::translate(viewMatrix, glm::vec3(screen_->getWidthFloating() * 0.5f, -screen_->getHeightFloating() * 0.5f, 0.0f));

		finalMatrix_ = projectionMatrix * viewMatrix;
	}
	else
	{
		Matrix viewMatrix = glm::lookAt<float>(from_, to_, up_);

		float aspectRatio = screen_->getWidthFloating() / screen_->getHeightFloating();

		Matrix projectionMatrix = glm::perspective<float>(60.0f / 57.297f, aspectRatio, 1.0f, 500.0f);

		finalMatrix_ = projectionMatrix * viewMatrix * Matrix(1.0f);
	}
}

void Camera::SetTarget(glm::vec3 target)
{
	to_ = target;
	azimuth_ = 1.57079f;
	/*if(to_.y < 0) to_.y = 0;
	if(to_.y > world.height_) to_.y = world.height_;
	if(to_.x < 0) to_.x = 0;
	if(to_.x > world.width_) to_.x = world.width_;*/
}

void Camera::PushForward(float impulse)
{
	auto direction = Float3(cos(azimuth_), sin(azimuth_), 0.0f) * impulse;

	Drag(direction);
}

void Camera::Drag(glm::vec3 targetDelta)
{
	driftImpulse_ += targetDelta;
}

void Camera::Zoom(float impulse)
{
	scrollImpulse_ -= impulse;
}

void Camera::Spin(float impulse)
{
	spinImpulse_ -= impulse;
}

void Camera::Update()
{
	if(type_ == CameraTypes::PERSPECTIVE)
	{
		azimuth_ += spinImpulse_;

		while(azimuth_ > 6.2831f)
			azimuth_ -= 6.2831f;

		while(azimuth_ < 0.0f)
			azimuth_ += 6.2831f;

		spinImpulse_ *= 0.95f;

		to_ += driftImpulse_;

		driftImpulse_ *= 0.9f;

		viewDistance_ += scrollImpulse_;

		if(viewDistance_ < 1.0f)
			viewDistance_ = 1.0f;

		if(viewDistance_ > 600.0f)
			viewDistance_ = 600.0f;

		scrollImpulse_ *= 0.9f;
	}

	viewDirection_ = glm::vec3(sin(zenith_) * cos(azimuth_), sin(zenith_) * sin(azimuth_), cos(zenith_));

	from_ = to_ + (viewDirection_ * viewDistance_);

	glm::vec2 horizontal(viewDirection_.x, viewDirection_.y);

	float vertical = glm::length(horizontal);

	horizontal = -glm::normalize(horizontal) * viewDirection_.z;

	up_ = glm::vec3(horizontal.x, horizontal.y, vertical);

	ComputeMatrix();
}

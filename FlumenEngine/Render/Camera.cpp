#include <stdlib.h>

#include <glm/gtx/transform.hpp>

#include "FlumenCore/Time.hpp"

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

	isAnimated = false;
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

	isAnimated = false;
}

Camera::Camera(Screen* screen)
{
	screen_ = screen;

	to_ = Position3(0.0f);

	type_ = CameraTypes::ORTHO;

	zoomFactor_ = 1.0f;

	isAnimated = false;
}

Matrix & Camera::GetMatrix()
{
	return finalMatrix_;
}

void Camera::ComputeMatrix()
{
	if(type_ == CameraTypes::ORTHO)
	{
		Position3 screenCenter = to_ + Position3(-screen_->getWidthFloating() * 0.5f * zoomFactor_, -screen_->getHeightFloating() * 0.5f * zoomFactor_, 0.0f);

		glm::mat4 projectionMatrix = glm::ortho<float> (0.0f, screen_->getWidthFloating() * zoomFactor_, screen_->getHeightFloating() * zoomFactor_, 0.0f, 0.1f, 10.0f);

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

void Camera::SetTarget(Position3 target, Float duration)
{
	startPosition = to_;
	endPosition = target;

	isAnimated = true;
	animationTime = 0.0f;
	animationDuration = duration;
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
	if(type_ == CameraTypes::PERSPECTIVE)
	{
		scrollImpulse_ -= impulse;
	}
	else
	{
		zoomFactor_ *= impulse;

		if(isZoomLimited_ == true)
		{
			zoomFactor_ = utility::Clamp(zoomFactor_, zoomLimit_.x, zoomLimit_.y);
		}
	}
}

void Camera::ZoomDynamically(float impulse)
{
	scrollImpulse_ -= impulse;
}

void Camera::Spin(float impulse)
{
	spinImpulse_ -= impulse;
}

void Camera::Update()
{
	if(isAnimated)
	{
		if(animationTime > animationDuration)
		{
			animationDuration = animationTime;
			isAnimated = false;
		}

		to_ = startPosition + (endPosition - startPosition) * (animationTime / animationDuration);

		animationTime += Time::GetDelta();
	}

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

		if(viewDistance_ < 1.0f)
			viewDistance_ = 1.0f;

		if(viewDistance_ > 600.0f)
			viewDistance_ = 600.0f;
	}

	if(isZoomingDynamically == true)
	{
		viewDistance_ += scrollImpulse_;

		zoomFactor_ *= (1.0f + scrollImpulse_);

		if(isZoomLimited_ == true)
		{
			zoomFactor_ = utility::Clamp(zoomFactor_, zoomLimit_.x, zoomLimit_.y);
		}

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

void Camera::Translate(Direction3 delta)
{
	to_ += delta;
}

Position2 Camera::GetScreenPosition(Position3 position) const
{
	auto returnPosition = finalMatrix_ * Float4(position, 1.0f);
	//returnPosition.x /= returnPosition.w;
	//returnPosition.y /= returnPosition.w;
	//returnPosition.z /= returnPosition.w;

	float x = (returnPosition.x / 2.0f) * screen_->getWidthFloating();
	float y = (-returnPosition.y / 2.0f) * screen_->getHeightFloating();

	return Position2(x, y);
}

Position2 Camera::GetWorldPosition(Position2 position) 
{
	position *= 2.0f;

	position.x /= screen_->getWidthFloating();
	position.y /= -screen_->getHeightFloating();

	auto returnPosition = glm::inverse(finalMatrix_) * Float4(position, 0.0f, 1.0f);


	return Position2(returnPosition.x, returnPosition.y);
}
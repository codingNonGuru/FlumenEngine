#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenEngine/Core/Types.hpp"

class World;
class Screen;

class Camera
{
	CameraTypes type_;

	float viewDistance_;

	glm::vec3 from_;

	glm::vec3 to_;

	glm::vec3 up_;

	glm::vec3 viewDirection_;

	glm::vec3 pivot_;

	float zenith_;

	float azimuth_;

	float scrollImpulse_ {0.0f};

	float spinImpulse_;

	glm::vec3 driftImpulse_;

	glm::mat4x4 finalMatrix_;

	Screen* screen_;

	float zoomFactor_;

	bool isZoomingDynamically {false};

	bool isZoomLimited_ {false};

	Scale2 zoomLimit_;

	bool isAnimated;

	Float animationTime;

	Float animationDuration;

	Position3 startPosition;

	Position3 endPosition;

	void ComputeMatrix();

public:
	Camera();

	Camera(Screen*, Position3, float, float, float);

	Camera(Screen*, Position3, Position3);

	Camera(Screen*);

	Matrix & GetMatrix();

	void SetTarget(glm::vec3);

	void SetTarget(glm::vec2);

	void SetTarget(Position3, Float);

	void SetTarget(Position2, Float);

	void Drag(glm::vec3);

	void Zoom(float impulse);

	void EnableDynamicZooming() {isZoomingDynamically = true;}

	void ZoomDynamically(float);

	void LimitZoom(Scale2 limit) {isZoomLimited_ = true; zoomLimit_ = limit;}

	void Spin(float impulse);

	void PushForward(float);

	void SetTargetAsPivot();

	Position3 GetPosition() const {return from_;}

	Position3 GetTarget() const {return to_;}

	float GetAzimuth() const {return azimuth_;}

	float GetViewDistance() const {return viewDistance_;}

	float GetZoomFactor() const {return zoomFactor_;}

	void Update();

	void Translate(Direction3);

	Position2 GetScreenPosition(Position3) const;

	Position2 GetWorldPosition(Position2);
};

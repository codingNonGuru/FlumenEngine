#include <glm/gtx/transform.hpp>

#include "Light.hpp"

Light::Light() {}

Light::Light(Float3 direction)
{
	direction_ = glm::normalize(direction);
}

Float3 Light::GetDirection() const
{
	return direction_;
}

Matrix Light::GetShadowMatrix(Float size, Float3 target) const
{
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-size, size, -size, size, -size, size);
	glm::mat4 depthViewMatrix = glm::lookAt<float>(direction_ + target, target, glm::vec3(1.0f));
	return depthProjectionMatrix * depthViewMatrix;
}

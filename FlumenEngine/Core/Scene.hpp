#pragma once

#include "Object.hpp"

class Camera;
class Light;

class Scene : public Object
{
protected:
	Camera* camera_;

	Light* light_;

public:
	virtual void Initialize();

	virtual void Render();
};

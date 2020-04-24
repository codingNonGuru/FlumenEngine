#pragma once

#include "Object.hpp"

class Camera;
class Light;

class Scene : public Object
{
	friend class SceneManager;
	
protected:
	Camera* camera_;

	Light* light_;

	Scene();

	virtual void Initialize() = 0;

	virtual void Render() = 0;
};

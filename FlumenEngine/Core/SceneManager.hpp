#pragma once

#include "FlumenCore/Conventions.hpp"

class Scene;

enum class Scenes;

class SceneManager
{
	friend class Scene;

	static Array <Scene*> scenes;

	static Scene * AddScene(Scene*);

public:
	static void Update();

	static void Render();
};

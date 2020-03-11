#pragma once

#include "FlumenCore/Conventions.hpp"

class Scene;

enum class Scenes;

class SceneManager
{
	static Map <Scene*, Scenes> scenes_;

public:
	static Scene * Get(Scenes);

	static Scene * AddScene(Scene*, Scenes);

	static void Update();

	static void Render();
};

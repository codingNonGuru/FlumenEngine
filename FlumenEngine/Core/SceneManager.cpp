#include "FlumenEngine/Core/SceneManager.hpp"

#include "Scene.hpp"

#define MAXIMUM_SCENE_COUNT 32

Array <Scene*> SceneManager::scenes = Array <Scene*> (MAXIMUM_SCENE_COUNT);

Scene * SceneManager::AddScene(Scene* scene)
{
	auto scenePointer = scenes.Allocate();
	if(!scenePointer)
		return nullptr;

	*scenePointer = scene;

	return *scenePointer;
}

void SceneManager::Update()
{
	for(auto sceneIterator = scenes.GetStart(); sceneIterator != scenes.GetEnd(); ++sceneIterator)
	{
		auto scene = *sceneIterator;
		if(!scene)
			continue;

		if(!scene->IsGloballyActive())
			continue;

		scene->Update();
	}
}

void SceneManager::Render()
{
	for(auto sceneIterator = scenes.GetStart(); sceneIterator != scenes.GetEnd(); ++sceneIterator)
	{
		auto scene = *sceneIterator;
		if(scene == nullptr)
			continue;

		if(!scene->IsGloballyActive())
			continue;

		scene->Render();
	}
}

#include "Scene.hpp"
#include "FlumenEngine/Core/SceneManager.hpp"

Scene::Scene()
{
    SceneManager::AddScene(this);
}
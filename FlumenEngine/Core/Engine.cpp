#include <GL/glew.h>
#include "SDL2/SDL.h"

#include "FlumenEngine/Core/Engine.hpp"
#include "FlumenEngine/Render/Screen.hpp"
#include "FlumenEngine/Render/RenderManager.hpp"
#include "FlumenEngine/Render/Window.hpp"
#include "FlumenEngine/Core/InputHandler.hpp"
#include "FlumenEngine/Core/AssetManager.hpp"
#include "FlumenEngine/Interface/Interface.hpp"
#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Core/SceneManager.hpp"
#include "TaskManager.hpp"
#include "FlumenCore/Time.hpp"
#include "FlumenEngine/Utility/Perlin.hpp"

bool Engine::isRunning_ = false;

Screen* Engine::screen_ = nullptr;

Delegate Engine::OnInitialize_ = Delegate();

Delegate Engine::OnGameLoopStart_ = Delegate();

Delegate Engine::OnShutDown_ = Delegate();

void Engine::Initialize()
{
	AssetManager::Initialize();

	screen_ = new Screen(2560, 1440);

	RenderManager::Initialize();

	AssetManager::LoadAssets();

	InputHandler::Initialize();

	OnInitialize_.Invoke();
}

void Engine::StartGameLoop()
{
	isRunning_ = true;

	Time::Update();

	OnGameLoopStart_.Invoke();

	while(isRunning_)
	{
		Time::Update();

		InputHandler::Update();

		if(InputHandler::IsPressed(SDL_SCANCODE_ESCAPE))
		{
			ShutDown();
		}

		SceneManager::Update();

		TaskManager::Update();

		Interface::Update();

		RenderManager::Update();
	}
}

void Engine::ShutDown()
{
	isRunning_ = false;

	OnShutDown_.Invoke();

	SDL_Quit();
}

Screen* Engine::GetScreen()
{
	return screen_;
}

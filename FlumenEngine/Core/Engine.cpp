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
#include "FlumenEngine/Thread/ThreadManager.h"
#include "TaskManager.hpp"
#include "FlumenCore/Time.hpp"
#include "FlumenEngine/Utility/Perlin.hpp"

bool Engine::isRunning_ = false;

Screen* Engine::screen_ = nullptr;

Delegate Engine::OnInitializeStarted = Delegate();

Delegate Engine::OnInitializeEnded = Delegate();

Delegate Engine::OnGameLoopStarted = Delegate();

Delegate Engine::OnInterfaceUpdateStarted = Delegate();

Delegate Engine::OnShutDown = Delegate();

void Engine::Initialize(Size size)
{
	OnInitializeStarted.Invoke();

	AssetManager::Initialize();

	screen_ = new Screen(size);

	RenderManager::Initialize();

	AssetManager::LoadAssets();

	InputHandler::Initialize();

	Interface::Get()->Initialize();

	OnInitializeEnded.Invoke();

	StartGameLoop();
}

void Engine::StartGameLoop()
{
	isRunning_ = true;

	Time::Update();

	OnGameLoopStarted.Invoke();

	while(isRunning_)
	{
		Time::Update();

		InputHandler::Update();

		if(InputHandler::IsPressed(SDL_SCANCODE_ESCAPE))
		{
			ShutDown();
		}

		engine::ThreadManager::Get()->Update();

		SceneManager::Update();

		TaskManager::Update();

		OnInterfaceUpdateStarted.Invoke();

		Interface::Get()->Update();

		RenderManager::Update();
	}
}

void Engine::ShutDown()
{
	isRunning_ = false;

	OnShutDown.Invoke();

	SDL_Quit();
}

Screen* Engine::GetScreen()
{
	return screen_;
}

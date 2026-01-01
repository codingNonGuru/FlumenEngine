#include <GL/glew.h>
#include "SDL2/SDL.h"

#include "FlumenCore/Time.hpp"

#include "FlumenEngine/Core/Engine.hpp"
#include "FlumenEngine/Core/FileManager.hpp"
#include "FlumenEngine/Core/ConfigManager.h"
#include "FlumenEngine/Render/Screen.hpp"
#include "FlumenEngine/Render/RenderManager.hpp"
#include "FlumenEngine/Render/Window.hpp"
#include "FlumenEngine/Core/InputHandler.hpp"
#include "FlumenEngine/Core/AssetManager.hpp"
#include "FlumenEngine/Interface/Interface.hpp"
#include "FlumenEngine/Core/SceneManager.hpp"
#include "FlumenEngine/Thread/ThreadManager.h"
#include "FlumenEngine/Core/StateManager.hpp"
#include "TaskManager.hpp"
#include "FlumenEngine/Config.h"

bool Engine::isRunning_ = false;

Screen* Engine::screen_ = nullptr;

Delegate Engine::OnInitializeStarted = Delegate();

Delegate Engine::OnInitializeEnded = Delegate();

Delegate Engine::OnGameLoopStarted = Delegate();

Delegate Engine::OnInterfaceUpdateStarted = Delegate();

Delegate Engine::OnLoopCycleStarted = Delegate();

Delegate Engine::OnLoopCycleEnded = Delegate();

Delegate Engine::OnShutDown = Delegate();

static float timeDelta = 0.0f;

static int tickCount = 0;

static float averageFps = 0.0f;

void Engine::Initialize()
{
	OnInitializeStarted.Invoke();

	engine::FileManager::Get();

	engine::ConfigManager::Get();

	AssetManager::Initialize();

	auto screenWidth = engine::ConfigManager::Get()->GetValue(engine::ConfigValues::DEFAULT_SCREEN_WIDTH).Integer;
	auto screenHeight = engine::ConfigManager::Get()->GetValue(engine::ConfigValues::DEFAULT_SCREEN_HEIGHT).Integer;
	screen_ = new Screen({screenWidth, screenHeight});

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
		OnLoopCycleStarted.Invoke();

		UpdateTime();	

		InputHandler::Update();

		if(InputHandler::IsPressed(SDL_SCANCODE_ESCAPE))
		{
			ShutDown();
		}

		engine::ThreadManager::Get()->Update();

		StateManager::Update();

		SceneManager::Update();

		TaskManager::Update();

		OnInterfaceUpdateStarted.Invoke();

		Interface::Get()->Update();

		RenderManager::Update();

		OnLoopCycleEnded.Invoke();
	}
}

void Engine::UpdateTime()
{
	Time::Update();

	timeDelta += Time::GetDelta();
	tickCount += 1;

	if(tickCount > 60)
	{
		averageFps = 60.0f / timeDelta;

		timeDelta = 0.0f;
		tickCount = 0;
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

float Engine::GetAverageFPS()
{
	return averageFps;
}
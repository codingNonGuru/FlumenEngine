#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenCore/Delegate/Delegate.hpp"

class Screen;
class RenderManager;
class Window;

class Engine
{
private:
	static bool isRunning_;

	static Screen* screen_;

public:
	static Delegate OnInitialize_;

	static Delegate OnGameLoopStart_;

	static Delegate OnShutDown_;

	static void Initialize();

	static void StartGameLoop();

	static void ShutDown();

	static Screen* GetScreen();
};

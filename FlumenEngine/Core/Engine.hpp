#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenCore/Delegate/Delegate.hpp"

class Screen;

class Engine
{
private:
	static bool isRunning_;

	static Screen* screen_;

	static void StartGameLoop();

public:
	static Delegate OnInitializeStarted;

	static Delegate OnInitializeEnded;

	static Delegate OnGameLoopStarted;

	static Delegate OnInterfaceUpdateStarted;

	static Delegate OnShutDown;

	static void Initialize();

	static void ShutDown();

	static Screen* GetScreen();
};

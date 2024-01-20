#pragma once

#include "SDL2/SDL.h"

#include "FlumenCore/Conventions.hpp"
#include "FlumenCore/Delegate/Delegate.hpp"
#include "FlumenCore/Delegate/Event.hpp"

class Engine;
class Delegate;

struct Mouse
{
	Position2 NormalizedPosition_;

	Position2 UnprocessedPosition_;

	bool CurrentRight_, CurrentLeft_;

	bool ScrollUp_, ScrollDown_;

	bool LastRight_, LastLeft_;

	Mouse() : NormalizedPosition_(Position2()), CurrentRight_(false), CurrentLeft_(false), LastRight_(false), LastLeft_(false), ScrollUp_(false), ScrollDown_(false) {}
};

class InputHandler
{
	static Mouse mouse_;

	static container::Array<int> currentKeys_;

	static container::Array<int> formerKeys_;

	static Map <Event, SDL_Scancode> onKeyPressedEvents;

	static Map <Event, SDL_Scancode> onKeyHeldEvents;

	static Map <Event, SDL_Scancode> onKeyReleasedEvents;

	static void UpdateMouse();

public:
	static Delegate OnInputUpdate;

	static Delegate OnRightMouseClick;

	static Delegate OnLeftMouseClick;

	static void Update();

	static bool IsPressed(int32_t);

	static bool WasPressed(int32_t);

	static void Initialize();

	static void RegisterEvent(SDL_Scancode, Event);

	static void RegisterContinualEvent(SDL_Scancode, Event);

	static void RegisterContinualEvent(SDL_Scancode, Event, Event);

	static void UnregisterEvent(SDL_Scancode);

	static void UnregisterContinualEvent(SDL_Scancode);

	static Position2 GetMousePosition(bool = true);

	static const Mouse & GetMouse();
};

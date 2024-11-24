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
public:
	enum SupportKeys
	{
		CTRL,
		SHIFT,
		ALT
	};

	struct Trigger
	{
		SDL_Scancode Key;

		bool HasCtrl {false};

		bool HasShift {false};

		bool HasAlt {false};

		Trigger() {}

		Trigger(SDL_Scancode key) : Key(key) {}

		Trigger(SDL_Scancode key, std::initializer_list <SupportKeys> supportKeys) : Key(key) 
		{
			HasCtrl = std::find(std::begin(supportKeys), std::end(supportKeys), SupportKeys::CTRL) != std::end(supportKeys);

			HasShift = std::find(std::begin(supportKeys), std::end(supportKeys), SupportKeys::SHIFT) != std::end(supportKeys);

			HasAlt = std::find(std::begin(supportKeys), std::end(supportKeys), SupportKeys::ALT) != std::end(supportKeys);
		}

		bool operator== (const Trigger &other)
		{
			return this->Key == other.Key && this->HasCtrl == other.HasCtrl && this->HasShift == other.HasShift && this->HasAlt == other.HasAlt;
		}
	};

private:
	static Mouse mouse_;

	static container::Array <int> currentKeys_;

	static container::Array <int> formerKeys_;

	static Map <Event, Trigger> onKeyPressedEvents;

	static Map <Event, Trigger> onKeyHeldEvents;

	static Map <Event, Trigger> onKeyReleasedEvents;

	static Delegate OnMouseScrollUp;

	static Delegate OnMouseScrollDown;

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

	static void RegisterEvent(Trigger, Event);

	static void RegisterContinualEvent(Trigger, Event);

	static void RegisterContinualEvent(Trigger, Event, Event);

	static void UnregisterEvent(Trigger);

	static void UnregisterContinualEvent(Trigger);

	static void RegisterScrollUpEvent(Event);

	static void UnregisterScrollUpEvent(Event);

	static void RegisterScrollDownEvent(Event);

	static void UnregisterScrollDownEvent(Event);

	static Position2 GetMousePosition(bool = true);

	static const Mouse & GetMouse();
};

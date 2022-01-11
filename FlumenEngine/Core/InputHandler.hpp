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

	Mouse() : NormalizedPosition_(Position2(0.0f, 0.0f)), CurrentRight_(false), CurrentLeft_(false), LastRight_(false), LastLeft_(false), ScrollUp_(false), ScrollDown_(false) {}
};

class InputHandler
{
	static Mouse mouse_;

	static container::Array<int> currentKeys_;

	static container::Array<int> formerKeys_;

	static Map <Delegate, SDL_Scancode> onKeyPressedEvents;

	static Map <Delegate, SDL_Scancode> onKeyHeldEvents;

	static void UpdateMouse();

public:
	static Delegate OnInputUpdate;

	static Delegate OnRightMouseClick;

	static Delegate OnLeftMouseClick;

	static void Update();

	static bool IsPressed(int32_t);

	static bool WasPressed(int32_t);

	static void Initialize();

	static void RegisterEvent(SDL_Scancode key, Event action)
	{
		auto event = onKeyPressedEvents.Get(key);
		if(event != nullptr)
		{
			*event += action;
			//event->Add(object, function);
			return;
		}

		event = onKeyPressedEvents.Add(key);
		//event->Add(object, function);
		*event += action;
	}

	static void UnregisterEvent(SDL_Scancode key, Event action)
	{
		auto event = onKeyPressedEvents.Get(key);
		if(event != nullptr)
		{
			*event -= action;
		}
	}

	static void RegisterContinualEvent(SDL_Scancode key, Event action)
	{
		auto event = onKeyHeldEvents.Get(key);
		if(event != nullptr)
		{
			*event += action;
			//event->Add(object, function);
			return;
		}

		event = onKeyHeldEvents.Add(key);
		//event->Add(object, function);
		*event += action;
	}

	static void UnregisterContinualEvent(SDL_Scancode key, Event action)
	{
		auto event = onKeyHeldEvents.Get(key);
		if(event != nullptr)
		{
			*event -= action;
		}
	}

	/*template <class ClassType, class ObjectType>
	static void RegisterEvent(SDL_Scancode key, ObjectType* object, void(ClassType::*function)())
	{
		auto event = onKeyPressedEvents.Get(key);
		if(event != nullptr)
		{
			*event += {object, function};
			//event->Add(object, function);
			return;
		}

		event = onKeyPressedEvents.Add(key);
		event->Add(object, function);
	}

	static void RegisterEvent(SDL_Scancode key, void(*function)())
	{
		auto event = onKeyPressedEvents.Get(key);
		if(event != nullptr)
		{
			event->Add(function);
			return;
		}

		event = onKeyPressedEvents.Add(key);
		event->Add(function);
	}

	template <class ClassType, class ObjectType>
	static void UnregisterEvent(SDL_Scancode key, ObjectType* object, void(ClassType::*function)())
	{
		auto event = onKeyPressedEvents.Get(key);
		if(event != nullptr)
		{
			event->Remove(object, function);
			return;
		}
	}

	static void UnregisterEvent(SDL_Scancode key, void(*function)())
	{
		auto event = onKeyPressedEvents.Get(key);
		if(event != nullptr)
		{
			event->Remove(function);
			return;
		}
	}*/

	static Position2 GetMousePosition(bool = true);

	static const Mouse & GetMouse();
};

#include <chrono>

#include "FlumenEngine/Core/InputHandler.hpp"

#include "FlumenEngine/Core/Engine.hpp"
#include "FlumenEngine/Render/Screen.hpp"

#define REGISTER_FAIL_MESSAGE "Input handler is trying to overwrite event.\n"

static const auto KEY_MAP_SIZE = 64;

static const auto DOUBLE_CLICK_INTERVAL = 200;

Mouse InputHandler::mouse_ = Mouse();

container::Array<int> InputHandler::currentKeys_ = container::Array<int>();

container::Array<int> InputHandler::formerKeys_ = container::Array<int>();

Map <Event, InputHandler::Trigger> InputHandler::onKeyPressedEvents = Map <Event, Trigger> ();

Map <Event, InputHandler::Trigger> InputHandler::onKeyHeldEvents = Map <Event, Trigger> ();

Map <Event, InputHandler::Trigger> InputHandler::onKeyReleasedEvents = Map <Event, Trigger> ();

Delegate InputHandler::OnInputUpdate = Delegate();

Delegate InputHandler::OnRightMouseClick = Delegate();

Delegate InputHandler::OnLeftMouseClick = Delegate();

Delegate InputHandler::OnMouseScrollUp = Delegate();

Delegate InputHandler::OnMouseScrollDown = Delegate();

Delegate InputHandler::OnDoubleClick = Delegate();

static auto leftClickTimeStamp = std::chrono::steady_clock::now();

int64_t milisecondCount;

void InputHandler::Initialize()
{
	currentKeys_.Initialize(KEY_MAP_SIZE);

	formerKeys_.Initialize(KEY_MAP_SIZE);

	onKeyPressedEvents.Initialize(KEY_MAP_SIZE, Event());

	onKeyHeldEvents.Initialize(KEY_MAP_SIZE, Event());

	onKeyReleasedEvents.Initialize(KEY_MAP_SIZE, Event());
}

void InputHandler::Update()
{
	formerKeys_.Reset();

	for(auto key = currentKeys_.GetStart(); key != currentKeys_.GetEnd(); ++key)
		*formerKeys_.Allocate() = *key;

	currentKeys_.Reset();

	UpdateMouse();

	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_MOUSEBUTTONDOWN)
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				mouse_.CurrentLeft_ = true;
			}
			else if(event.button.button == SDL_BUTTON_RIGHT)
				mouse_.CurrentRight_ = true;
		}
		else if(event.type == SDL_MOUSEWHEEL)
		{
			if(event.wheel.y < 0)
				mouse_.ScrollDown_ = true;
			else if(event.wheel.y > 0)
				mouse_.ScrollUp_ = true;
		}
	}

	int keyCount = 0;
	const Uint8* keyStates = SDL_GetKeyboardState(&keyCount);

	int keyIndex = 0;
	for(auto key = keyStates; key != keyStates + keyCount; ++key, ++keyIndex)
	{
		if(*key)
			*currentKeys_.Allocate() = keyIndex;
	}

	SDL_PumpEvents();

	OnInputUpdate.Invoke();

	if(mouse_.CurrentRight_)
	{
		OnRightMouseClick.Invoke();
	}

	if(mouse_.CurrentLeft_)
	{
		OnLeftMouseClick.Invoke();

		auto stamp = std::chrono::steady_clock::now();

		auto deltaTime = stamp - leftClickTimeStamp;

		leftClickTimeStamp = stamp;
		
		milisecondCount = std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count();

		if(milisecondCount < DOUBLE_CLICK_INTERVAL)
		{
			OnDoubleClick.Invoke();
		}
	}

	if(mouse_.ScrollUp_)
	{
		OnMouseScrollUp.Invoke();
	}
	else if(mouse_.ScrollDown_)
	{
		OnMouseScrollDown.Invoke();
	}

	formerKeys_.Find(SDL_SCANCODE_LCTRL);

	for(auto currentKey = currentKeys_.GetStart(); currentKey != currentKeys_.GetEnd(); ++currentKey)
	{
		bool wasPressed = formerKeys_.Find(*currentKey) != nullptr;
		if(wasPressed == true)
		{
			for(auto key = onKeyHeldEvents.GetFirstKey(); key != onKeyHeldEvents.GetLastKey(); ++key)
			{
				if(key->Key != *currentKey)
					continue;

				bool isCtrlAlsoPressed = formerKeys_.Find(SDL_SCANCODE_LCTRL) != nullptr && currentKeys_.Find(SDL_SCANCODE_LCTRL) != nullptr && key->HasCtrl == true;
				bool isPressedWithoutCtrl = formerKeys_.Find(SDL_SCANCODE_LCTRL) == nullptr && currentKeys_.Find(SDL_SCANCODE_LCTRL) == nullptr && key->HasCtrl == false;

				bool isShiftAlsoPressed = formerKeys_.Find(SDL_SCANCODE_LSHIFT) != nullptr && currentKeys_.Find(SDL_SCANCODE_LSHIFT) != nullptr && key->HasShift == true;
				bool isPressedWithoutShift = formerKeys_.Find(SDL_SCANCODE_LSHIFT) == nullptr && currentKeys_.Find(SDL_SCANCODE_LSHIFT) == nullptr && key->HasShift == false;

				bool isPressedWithoutSupportKeys = isPressedWithoutShift && isPressedWithoutCtrl;

				if(isCtrlAlsoPressed || isShiftAlsoPressed || isPressedWithoutSupportKeys)
				{
					auto event = onKeyHeldEvents.Get(*key);
					if(event != nullptr && event->IsValid())
					{
						event->Invoke();
					}
				}
			}

			continue;
		}

		for(auto key = onKeyPressedEvents.GetFirstKey(); key != onKeyPressedEvents.GetLastKey(); ++key)
		{
			if(key->Key != *currentKey)
				continue;

			bool isCtrlAlsoPressed = key->HasCtrl == true && currentKeys_.Find(SDL_SCANCODE_LCTRL) != nullptr;
			bool isPressedWithoutCtrl = key->HasCtrl == false && currentKeys_.Find(SDL_SCANCODE_LCTRL) == nullptr;

			bool isShiftAlsoPressed = key->HasShift == true && currentKeys_.Find(SDL_SCANCODE_LSHIFT) != nullptr;
			bool isPressedWithoutShift = key->HasShift == false && currentKeys_.Find(SDL_SCANCODE_LSHIFT) == nullptr;

			bool isPressedWithoutSupportKeys = isPressedWithoutShift && isPressedWithoutCtrl;

			if(isCtrlAlsoPressed || isShiftAlsoPressed || isPressedWithoutSupportKeys)
			{
				auto event = onKeyPressedEvents.Get(*key);
				if(event == nullptr)
					continue;

				if(event->IsValid() == false)
					continue;

				event->Invoke();
			}
		}
	}

	for(auto &formerKey : formerKeys_)
	{
		bool isCurrentlyPressed = false;
		
		for(auto &currentKey : currentKeys_)
		{
			if(formerKey == currentKey)
			{
				isCurrentlyPressed = true;
				break;
			}
		}

		if(isCurrentlyPressed == true)
			continue;

		for(auto key = onKeyReleasedEvents.GetFirstKey(); key != onKeyReleasedEvents.GetLastKey(); ++key)
		{
			if(key->Key != formerKey)
				continue;

			bool isCtrlAlsoPressed = key->HasCtrl == true && formerKeys_.Find(SDL_SCANCODE_LCTRL) != nullptr;
			bool isPressedWithoutCtrl = key->HasCtrl == false && formerKeys_.Find(SDL_SCANCODE_LCTRL) == nullptr;

			bool isShiftAlsoPressed = key->HasShift == true && formerKeys_.Find(SDL_SCANCODE_LSHIFT) != nullptr;
			bool isPressedWithoutShift = key->HasShift == false && formerKeys_.Find(SDL_SCANCODE_LSHIFT) == nullptr;

			bool isPressedWithoutSupportKeys = isPressedWithoutShift && isPressedWithoutCtrl;

			if(isCtrlAlsoPressed || isShiftAlsoPressed || isPressedWithoutSupportKeys)
			{
				auto event = onKeyReleasedEvents.Get(*key);
				if(event == nullptr)
					continue;

				if(event->IsValid() == false)
					continue;

				event->Invoke();
			}
		}
	}
}

void InputHandler::UpdateMouse()
{
	mouse_.ScrollUp_ = false;
	mouse_.ScrollDown_ = false;

	mouse_.LastLeft_ = mouse_.CurrentLeft_;
	mouse_.LastRight_ = mouse_.CurrentRight_;

	mouse_.CurrentRight_ = false;
	mouse_.CurrentLeft_ = false;

	int x, y;
	unsigned int mouseFlags = SDL_GetMouseState(&x, &y);
	mouse_.NormalizedPosition_.x = x - Engine::GetScreen()->getWidthInteger() / 2;
	mouse_.NormalizedPosition_.y = y - Engine::GetScreen()->getHeightInteger() / 2;

	mouse_.UnprocessedPosition_.x = x;
	mouse_.UnprocessedPosition_.y = y;
}

const Mouse & InputHandler::GetMouse()
{
	return mouse_;
}

Position2 InputHandler::GetMousePosition(bool isNormalized)
{
	return isNormalized ? mouse_.NormalizedPosition_ : mouse_.UnprocessedPosition_;
}

bool InputHandler::IsPressed(int32_t key)
{
	for(auto currentKey = currentKeys_.GetStart(); currentKey != currentKeys_.GetEnd(); ++currentKey)
		if(*currentKey == key)
			return true;

	return false;
}

bool InputHandler::WasPressed(int32_t key)
{
	for(auto currentKey = currentKeys_.GetStart(); currentKey != currentKeys_.GetEnd(); ++currentKey)
	{
		if(*currentKey != key)
			continue;

		for(auto formerKey = formerKeys_.GetStart(); formerKey != formerKeys_.GetEnd(); ++formerKey)
		{
			if(*formerKey == key)
				return false;
		}

		return true;
	}

	return false;
}

void InputHandler::RegisterEvent(SDL_Scancode key, Event action)
{
	RegisterEvent(Trigger{key}, action);
}

void InputHandler::UnregisterEvent(SDL_Scancode key)
{
	UnregisterEvent(Trigger{key});
}

void InputHandler::RegisterContinualEvent(SDL_Scancode key, Event action)
{
	RegisterContinualEvent(Trigger{key}, action);
}

void InputHandler::RegisterContinualEvent(SDL_Scancode key, Event pressAction, Event releaseAction)
{
	RegisterContinualEvent(Trigger{key}, pressAction, releaseAction);
}

void InputHandler::UnregisterContinualEvent(SDL_Scancode key)
{
	UnregisterContinualEvent(Trigger{key});
}

void InputHandler::RegisterEvent(Trigger trigger, Event action)
{
	auto event = onKeyPressedEvents.Get(trigger);
	if(event == nullptr)
	{
		event = onKeyPressedEvents.Add(trigger);
	}
	else
	{
		auto assertCondition = event->IsValid() == false;
		assert(assertCondition && REGISTER_FAIL_MESSAGE);
	}

	*event = action;
}

void InputHandler::UnregisterEvent(Trigger trigger)
{
	auto event = onKeyPressedEvents.Get(trigger);
	if(event != nullptr)
	{
		event->Clear();
	}
}

void InputHandler::RegisterContinualEvent(Trigger trigger, Event action)
{
	auto event = onKeyHeldEvents.Get(trigger);
	if(event == nullptr)
	{
		event = onKeyHeldEvents.Add(trigger);
	}
	else
	{
		auto assertCondition = event->IsValid() == false;
		assert(assertCondition && REGISTER_FAIL_MESSAGE);
	}

	*event = action;
}

void InputHandler::RegisterContinualEvent(Trigger trigger, Event pressAction, Event releaseAction)
{
	auto event = onKeyPressedEvents.Get(trigger);
	if(event == nullptr)
	{
		event = onKeyPressedEvents.Add(trigger);
	}
	else
	{
		auto assertCondition = event->IsValid() == false;
		assert(assertCondition && REGISTER_FAIL_MESSAGE);
	}

	*event = pressAction;

	event = onKeyReleasedEvents.Get(trigger);
	if(event == nullptr)
	{
		event = onKeyReleasedEvents.Add(trigger);
	}
	else
	{
		auto assertCondition = event->IsValid() == false;
		assert(assertCondition && REGISTER_FAIL_MESSAGE);
	}

	*event = releaseAction;
}

void InputHandler::UnregisterContinualEvent(Trigger trigger)
{
	auto event = onKeyHeldEvents.Get(trigger);
	if(event != nullptr)
	{
		event->Clear();
	}

	event = onKeyPressedEvents.Get(trigger);
	if(event != nullptr)
	{
		event->Clear();
	}

	event = onKeyReleasedEvents.Get(trigger);
	if(event != nullptr)
	{
		event->Clear();
	}
}

void InputHandler::RegisterScrollUpEvent(Event event)
{
	OnMouseScrollUp += event;
}

void InputHandler::UnregisterScrollUpEvent(Event event)
{
	OnMouseScrollUp -= event;
}

void InputHandler::RegisterScrollDownEvent(Event event)
{
	OnMouseScrollDown += event;
}

void InputHandler::UnregisterScrollDownEvent(Event event)
{
	OnMouseScrollDown -= event;
}

void InputHandler::RegisterDoubleClickEvent(Event event)
{
	OnDoubleClick += event;
}

void InputHandler::UnregisterDoubleClickEvent(Event event)
{
	OnDoubleClick -= event;
}
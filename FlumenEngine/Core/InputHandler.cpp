#include "FlumenEngine/Core/InputHandler.hpp"

#include "FlumenEngine/Core/Engine.hpp"
#include "FlumenEngine/Render/Screen.hpp"

static const auto REGISTER_FAIL_MESSAGE = "Input handler is trying to overwrite event.\n";

static const auto KEY_MAP_SIZE = 64;

Mouse InputHandler::mouse_ = Mouse();

container::Array<int> InputHandler::currentKeys_ = container::Array<int>();

container::Array<int> InputHandler::formerKeys_ = container::Array<int>();

Map <Event, SDL_Scancode> InputHandler::onKeyPressedEvents = Map <Event, SDL_Scancode> ();

Map <Event, SDL_Scancode> InputHandler::onKeyHeldEvents = Map <Event, SDL_Scancode> ();

Map <Event, SDL_Scancode> InputHandler::onKeyReleasedEvents = Map <Event, SDL_Scancode> ();

Delegate InputHandler::OnInputUpdate = Delegate();

Delegate InputHandler::OnRightMouseClick = Delegate();

Delegate InputHandler::OnLeftMouseClick = Delegate();

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
				mouse_.CurrentLeft_ = true;
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
	}

	for(auto currentKey = currentKeys_.GetStart(); currentKey != currentKeys_.GetEnd(); ++currentKey)
	{
		bool wasPressed = false;
		for(auto formerKey = formerKeys_.GetStart(); formerKey != formerKeys_.GetEnd(); ++formerKey)
		{
			if(*formerKey == *currentKey)
			{
				wasPressed = true;
				break;
			}
		}

		if(wasPressed)
		{
			auto event = onKeyHeldEvents.Get((SDL_Scancode)*currentKey);
			if(event != nullptr && event->IsValid())
			{
				event->Invoke();
			}

			continue;
		}

		auto event = onKeyPressedEvents.Get((SDL_Scancode)*currentKey);
		if(event == nullptr)
			continue;

		if(event->IsValid() == false)
			continue;

		event->Invoke();
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

		auto event = onKeyReleasedEvents.Get((SDL_Scancode)formerKey);
		if(event == nullptr)
			continue;

		if(event->IsValid() == true)
		{
			event->Invoke();
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
	auto event = onKeyPressedEvents.Get(key);
	if(event == nullptr)
	{
		event = onKeyPressedEvents.Add(key);
	}
	else
	{
		auto assertCondition = event->IsValid() == false;
		assert(assertCondition && REGISTER_FAIL_MESSAGE);
	}

	*event = action;
}

void InputHandler::UnregisterEvent(SDL_Scancode key)
{
	auto event = onKeyPressedEvents.Get(key);
	if(event != nullptr)
	{
		event->Clear();
	}
}

void InputHandler::RegisterContinualEvent(SDL_Scancode key, Event action)
{
	auto event = onKeyHeldEvents.Get(key);
	if(event == nullptr)
	{
		event = onKeyHeldEvents.Add(key);
	}
	else
	{
		auto assertCondition = event->IsValid() == false;
		assert(assertCondition && REGISTER_FAIL_MESSAGE);
	}

	*event = action;
}

void InputHandler::RegisterContinualEvent(SDL_Scancode key, Event pressAction, Event releaseAction)
{
	auto event = onKeyPressedEvents.Get(key);
	if(event == nullptr)
	{
		event = onKeyPressedEvents.Add(key);
	}
	else
	{
		auto assertCondition = event->IsValid() == false;
		assert(assertCondition && REGISTER_FAIL_MESSAGE);
	}

	*event = pressAction;

	event = onKeyReleasedEvents.Get(key);
	if(event == nullptr)
	{
		event = onKeyReleasedEvents.Add(key);
	}
	else
	{
		auto assertCondition = event->IsValid() == false;
		assert(assertCondition && REGISTER_FAIL_MESSAGE);
	}

	*event = releaseAction;
}

void InputHandler::UnregisterContinualEvent(SDL_Scancode key)
{
	auto event = onKeyHeldEvents.Get(key);
	if(event != nullptr)
	{
		event->Clear();
	}

	event = onKeyPressedEvents.Get(key);
	if(event != nullptr)
	{
		event->Clear();
	}

	event = onKeyReleasedEvents.Get(key);
	if(event != nullptr)
	{
		event->Clear();
	}
}
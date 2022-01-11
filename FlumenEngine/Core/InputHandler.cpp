#include "FlumenEngine/Core/InputHandler.hpp"

#include "FlumenEngine/Core/Engine.hpp"
#include "FlumenEngine/Render/Screen.hpp"

Mouse InputHandler::mouse_ = Mouse();

container::Array<int> InputHandler::currentKeys_ = container::Array<int>();

container::Array<int> InputHandler::formerKeys_ = container::Array<int>();

Map <Delegate, SDL_Scancode> InputHandler::onKeyPressedEvents = Map <Delegate, SDL_Scancode> ();

Map <Delegate, SDL_Scancode> InputHandler::onKeyHeldEvents = Map <Delegate, SDL_Scancode> ();

Delegate InputHandler::OnInputUpdate = Delegate();

Delegate InputHandler::OnRightMouseClick = Delegate();

Delegate InputHandler::OnLeftMouseClick = Delegate();

void InputHandler::Initialize()
{
	currentKeys_.Initialize(64);

	formerKeys_.Initialize(64);

	onKeyPressedEvents.Initialize(64);

	onKeyHeldEvents.Initialize(64);
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

	for(auto currKey = currentKeys_.GetStart(); currKey != currentKeys_.GetEnd(); ++currKey)
	{
		bool wasPressed = false;
		for(auto formerKey = formerKeys_.GetStart(); formerKey != formerKeys_.GetEnd(); ++formerKey)
		{
			if(*formerKey == *currKey)
			{
				wasPressed = true;
				break;
			}
		}

		if(wasPressed)
		{
			auto event = onKeyHeldEvents.Get((SDL_Scancode)*currKey);
			if(event != nullptr)
				event->Invoke();

			continue;
		}

		auto event = onKeyPressedEvents.Get((SDL_Scancode)*currKey);
		if(event == nullptr)
			continue;

		event->Invoke();
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
	for(auto currKey = currentKeys_.GetStart(); currKey != currentKeys_.GetEnd(); ++currKey)
		if(*currKey == key)
			return true;

	return false;
}

bool InputHandler::WasPressed(int32_t key)
{
	for(auto currentKey = currentKeys_.GetStart(); currentKey != currentKeys_.GetEnd(); ++currentKey)
	{
		if(*currentKey != key)
			continue;

		for(auto formKey = formerKeys_.GetStart(); formKey != formerKeys_.GetEnd(); ++formKey)
		{
			if(*formKey == key)
				return false;
		}

		return true;
	}

	return false;
}

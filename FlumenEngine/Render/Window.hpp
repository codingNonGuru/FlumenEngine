#pragma once

class SDL_Window;
class Screen;

class Window
{
private:
	SDL_Window* window_;

	Screen* screen_;

public:
	Window(Screen*);

	void Initialize();

	void Refresh();
};

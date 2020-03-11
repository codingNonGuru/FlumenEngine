#pragma once

#include "FlumenCore/Conventions.hpp"

class Screen
{
private:
	int widthInteger_, heightInteger_;

	float widthFloating_, heightFloating_;

public:
	int getWidthInteger() const
	{
		return widthInteger_;
	}

	int getHeightInteger() const
	{
		return heightInteger_;
	}

	float getWidthFloating() const
	{
		return widthFloating_;
	}

	float getHeightFloating() const
	{
		return heightFloating_;
	}

	Screen() {}

	Screen(int width, int height)
	{
		Initialize(width, height);
	}

	void Initialize(int width, int height)
	{
		widthInteger_ = width;
		heightInteger_ = height;
		widthFloating_ = (float)width;
		heightFloating_ = (float)height;
	}

	Size GetSize()
	{
		return Size(widthInteger_, heightInteger_);
	}
};

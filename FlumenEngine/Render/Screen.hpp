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

	Screen(Size size)
	{
		Initialize(size);
	}

	void Initialize(Size size)
	{
		widthInteger_ = size.x;
		heightInteger_ = size.y;
		widthFloating_ = (float)size.x;
		heightFloating_ = (float)size.y;
	}

	Size GetSize()
	{
		return Size(widthInteger_, heightInteger_);
	}
};

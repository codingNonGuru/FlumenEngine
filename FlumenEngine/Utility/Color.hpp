#pragma once

#include "FlumenCore/Conventions.hpp"

class Color
{
public:
	float r_, g_, b_, a_;

	static float GetSaturation(Color);

	static Color SetSaturation(Color, float);

	static Color AddSaturation(Color, float);

	static Color Lighten(Color, float);

	Color();

	Color(float);

	Color(float, float, float);

	Color(float, float, float, float);

	Color(Float4);

	Color operator + (const Color&);

	Color operator * (float);

	Color operator - (Color&);

	operator Float4 ();

	static Color BLACK;

	static Color WHITE;

	static Color GRAY;

	static Color RED;

	static Color BLUE;

	static Color GREEN;

	static Color DARK_GREEN;

	static Color YELLOW;

	static Color CYAN;

	static Color ORANGE;

	static Color MAGENTA;
};
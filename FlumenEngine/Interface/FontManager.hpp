#pragma once

#include "FlumenCore/Conventions.hpp"

class Font;

class FontManager
{
private:
	static Map <Font*, Word> fonts_;

public:
	static void LoadFonts();

	static Map <Font*, Word> & GetFonts();

	static Font * GetFont(const char*);

	static Font * AddFont(Font*, const char*);
};

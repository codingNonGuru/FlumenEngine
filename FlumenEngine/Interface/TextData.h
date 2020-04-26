#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Utility/Color.hpp"

struct FontDescriptor
{
	Word Name;

	FontDescriptor() {}

	FontDescriptor(Word name, Word size) {Name << name << size;}

	operator Word() {return Name;}

	operator const char*() {return Name;}
};

struct TextData
{
    FontDescriptor Font;

    typedef Color TextColor;
    TextColor Color;

    Phrase Content;

    TextData() : Font(), Color(), Content() {}

    TextData(FontDescriptor font, TextColor color, Phrase content) : Font(font), Color(color), Content(content) {}

    TextData(FontDescriptor font, TextColor color) : Font(font), Color(color), Content() {}

    TextData(FontDescriptor font, Phrase content) : Font(font), Color(TextColor::BLACK), Content(content) {}

    TextData(FontDescriptor font) : Font(font), Color(TextColor::BLACK), Content() {}
};
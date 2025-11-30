#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Utility/Color.hpp"

struct FontDescriptor
{
	Word Name;

	FontDescriptor();

    FontDescriptor(Word);

	FontDescriptor(Word, Word);

	operator Word() {return Name;}

	operator const char*() {return Name;}
};

struct RowData
{
    int Count;

    int Size;
};

struct TextData
{
    FontDescriptor Font;

    typedef Color TextColor;
    TextColor PrimaryColor;

    TextColor SecondaryColor;

    Phrase Content;

    RowData *RowData = nullptr;

    TextData() : Font(), PrimaryColor(), Content() {}

    TextData(FontDescriptor font, TextColor color, Phrase content) : Font(font), PrimaryColor(color), Content(content) {}

    TextData(FontDescriptor font, TextColor primaryColor, TextColor secondaryColor, Phrase content) : Font(font), PrimaryColor(primaryColor), SecondaryColor(secondaryColor), Content(content) {}

    TextData(FontDescriptor font, TextColor primaryColor, TextColor secondaryColor, struct RowData *rowData) : Font(font), PrimaryColor(primaryColor), SecondaryColor(secondaryColor), Content(), RowData(rowData) {}

    TextData(FontDescriptor font, TextColor primaryColor, TextColor secondaryColor) : Font(font), PrimaryColor(primaryColor), SecondaryColor(secondaryColor), Content() {}

    TextData(FontDescriptor font, TextColor color) : Font(font), PrimaryColor(color), Content() {}

    TextData(FontDescriptor font, Phrase content) : Font(font), PrimaryColor(TextColor::BLACK), Content(content) {}

    TextData(FontDescriptor font) : Font(font), PrimaryColor(TextColor::BLACK), Content() {}
};
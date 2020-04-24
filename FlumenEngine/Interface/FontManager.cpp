#include "FontManager.hpp"

#include "FlumenEngine/Core/AssetManager.hpp"
#include "FlumenEngine/Interface/Font.hpp"
#include "FlumenEngine/Core/File.hpp"

#define MAXIMUM_FONT_COUNT 32

Map <Font*, Word> FontManager::fonts_ = Map <Font*, Word> (MAXIMUM_FONT_COUNT);

struct 
{
	Length Size;
	Word Name;
} sizeDatas[] = {{15, "Tiny"}, {20, "VerySmall"}, {25, "Small"}, {30, "Medium"}, {40, "Large"}, {50, "VeryLarge"}, {60, "Huge"}};

void FontManager::LoadFonts()
{
	auto files = AssetManager::GetFiles();

	for(File* file = files->GetStart(); file != files->GetEnd(); ++file)
	{
		auto fontExtension = ".ttf";

		auto extensionPosition = FindString(file->GetName(), fontExtension);
		if(extensionPosition == nullptr)
			continue;

		for(auto sizeData : sizeDatas)
		{
			Word fontName;
			fontName.Add(file->GetName(), extensionPosition - file->GetName());
			fontName.Add(sizeData.Name);

			auto font = fonts_.Add(fontName);
			if(font == nullptr)
				continue;

			*font = new Font(file, sizeData.Size);
		}
	}
}

Map <Font*, Word> & FontManager::GetFonts()
{
	return fonts_;
}

Font * FontManager::GetFont(const char* fontName)
{
	auto fontPointer = fonts_.Get(fontName);
	if(!fontPointer)
		return nullptr;

	return *fontPointer;
}

Font * FontManager::AddFont(Font* font, const char* fontName)
{
	auto fontPointer = fonts_.Add(fontName);
	if(!fontPointer)
		return nullptr;

	*fontPointer = font;

	return *fontPointer;
}

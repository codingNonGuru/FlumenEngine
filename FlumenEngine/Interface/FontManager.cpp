#include "FontManager.hpp"

#include "FlumenEngine/Core/AssetManager.hpp"
#include "FlumenEngine/Interface/Font.hpp"
#include "FlumenEngine/Core/File.hpp"

#define MAXIMUM_FONT_COUNT 16

Map <Font*, Word> FontManager::fonts_ = Map <Font*, Word> (MAXIMUM_FONT_COUNT);

void FontManager::LoadFonts()
{
	auto files = AssetManager::GetFiles();

	for(File* file = files->GetStart(); file != files->GetEnd(); ++file)
	{
		auto fontExtension = ".ttf";

		auto extensionPosition = FindString(file->GetName(), fontExtension);
		if(extensionPosition == nullptr)
			continue;

		Word fontName;
		fontName.Add(file->GetName(), extensionPosition - file->GetName());

		auto font = fonts_.Add(fontName);
		if(font == nullptr)
			continue;

		*font = new Font(file);
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

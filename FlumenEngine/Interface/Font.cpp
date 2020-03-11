#include "SDL2/SDL_ttf.h"

#include "Font.hpp"

#include "FlumenEngine/Core/File.hpp"
#include "FlumenEngine/Render/Texture.hpp"
#include "Glyph.hpp"

Font::Font()
{
	texture_ = nullptr;
}

Font::Font(File* file)
{
	TTF_Init();

	SDL_Color color;
	color.a = 255;
	const char* signs = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .:,;_";

	size_ = 70;

	TTF_Font* font = TTF_OpenFont(file->GetPath(), size_);

	ascent_ = (float)TTF_FontAscent(font);
	descent_ = (float)TTF_FontDescent(font);

	int signCount = 0;
	int width = 0, height = 0;
	for(char* sign = signs; *sign != 0; ++sign, ++signCount)
	{
		char symbol[] = {*sign, 0};
		SDL_Surface* letters = TTF_RenderUTF8_Blended_Wrapped(font, symbol, color, 800);
		width += letters->w;
		height = letters->h;
		SDL_FreeSurface(letters);
	}

	Grid <Byte> text(width + 4 - width % 4, height);
	for(auto t = text.GetStart(); t != text.GetEnd(); ++t)
		*t = 0;

	glyphs_.Initialize(128);

	for(int i = 0; i < glyphs_.GetCapacity(); ++i)
		glyphs_.Allocate();

	int horizontalAdvance = 0;
	for(char* sign = signs; *sign != 0; ++sign)
	{
		char symbol[] = {*sign, 0};
		SDL_Surface* letters = TTF_RenderUTF8_Blended_Wrapped(font, symbol, color, 800);
		int minx, maxx, miny, maxy, advance;
		TTF_GlyphMetrics(font, *sign, &minx, &maxx, &miny, &maxy, &advance);

		auto scale = Float2((float)letters->w, (float)letters->h);
		auto position = Float2((float)0, (float)0);
		auto textureOffset = Float2((float)horizontalAdvance / (float)text.GetWidth(), 0.0f);
		auto textureScale = Float2((float)letters->w / (float)text.GetWidth(), (float)letters->h / (float)text.GetHeight());

		auto glyph = glyphs_.Get(*sign);
		*glyph = Glyph(scale, letters->w, position, textureOffset, textureScale);
		glyph->MinimumX_ = minx;

		//std::cout<<*sign<<" "<<advance<<" "<<letters->w<<" "<<minx<<"\n";

		for(int x = 0; x < letters->w; ++x)
		{
			for(int y = 0; y < letters->h; ++y)
			{
				int index = x + y * letters->w;
				*text(horizontalAdvance + x, y) = ((SDL_Color*)letters->pixels + index)->a;
			}
		}
		horizontalAdvance += letters->w;
		SDL_FreeSurface(letters);
	}

	texture_ = new Texture(Size(text.GetWidth(), text.GetHeight()), TextureFormats::ONE_BYTE, &text);

	TTF_CloseFont(font);
}

Texture* Font::GetTexture()
{
	return texture_;
}

Glyph* Font::GetGlyph(char sign)
{
	int index = int(sign);
	return glyphs_.Get(index);
}

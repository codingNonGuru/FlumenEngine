#include "SDL2/SDL_ttf.h"

#include "Font.hpp"

#include "FlumenEngine/Core/File.hpp"
#include "FlumenEngine/Render/Texture.hpp"
#include "Glyph.hpp"

Font::Font()
{
	texture_ = nullptr;
}

Uint32 GetPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
        else
            return p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;       
    }
}

Uint8 GetAlpha(Uint32 pixel, SDL_PixelFormat *format)
{
	static Uint8 r, g, b, a;

	SDL_GetRGBA(pixel, format, &r, &g, &b, &a);

	return a;
}

Font::Font(File* file, Length size)
{
	TTF_Init();

	SDL_Color color;
	color.a = 255;
	const char* signs = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .:,;_!?-/+=()[]<>";

	TTF_Font* font = TTF_OpenFont(file->GetPath(), size);

	ascent_ = (float)TTF_FontAscent(font);
	descent_ = (float)TTF_FontDescent(font);

	int signCount = 0;
	int width = 0, height = 0;
	for(char* sign = (char *)signs; *sign != 0; ++sign, ++signCount)
	{
		char symbol[] = {*sign, 0};
		SDL_Surface* letters = TTF_RenderUTF8_Blended(font, symbol, color);
		width += letters->w;
		height = letters->h;

		SDL_FreeSurface(letters);
	}

	height_ = height;

	Grid <Byte> text(width + 4 - width % 4, height);
	for(auto t = text.GetStart(); t != text.GetEnd(); ++t)
		*t = 0;

	glyphs_.Initialize(128);

	for(int i = 0; i < glyphs_.GetCapacity(); ++i)
		glyphs_.Allocate();

	int horizontalAdvance = 0;
	for(char* sign = (char *)signs; *sign != 0; ++sign)
	{
		char symbol[] = {*sign, 0};
		SDL_Surface* letters = TTF_RenderUTF8_Blended(font, symbol, color);
		int minx, maxx, miny, maxy, advance;
		TTF_GlyphMetrics(font, *sign, &minx, &maxx, &miny, &maxy, &advance);

		auto scale = Float2((float)letters->w, (float)letters->h);
		auto position = Float2((float)0, (float)0);
		auto textureOffset = Float2((float)horizontalAdvance / (float)text.GetWidth(), 0.0f);
		auto textureScale = Float2((float)letters->w / (float)text.GetWidth(), (float)letters->h / (float)text.GetHeight());

		auto glyph = glyphs_.Get(*sign);
		*glyph = Glyph(scale, letters->w, position, textureOffset, textureScale);
		glyph->MinimumX_ = minx;

		for(int x = 0; x < letters->w; ++x)
		{
			for(int y = 0; y < letters->h; ++y)
			{
				int index = x + y * letters->w;
				*text(horizontalAdvance + x, y) = GetAlpha(GetPixel(letters, x, y), letters->format); //((SDL_Color*)letters->pixels + index)->a;
			}
		}
		
		horizontalAdvance += letters->w;
		SDL_FreeSurface(letters);
	}

	texture_ = new render::Texture(Size(text.GetWidth(), text.GetHeight()), TextureFormats::ONE_BYTE, &text);

	TTF_CloseFont(font);
}

render::Texture* Font::GetTexture()
{
	return texture_;
}

Glyph* Font::GetGlyph(char sign)
{
	int index = int(sign);
	return glyphs_.Get(index);
}

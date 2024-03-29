#pragma once

#include "FlumenCore/Conventions.hpp"

class File;
namespace render
{
	class Texture;
}
class Glyph;

class Font
{
	int size_;

	float ascent_;

	float descent_;

	int height_;

	render::Texture* texture_;

	Array <Glyph> glyphs_;

public:
	Font();

	Font(File*, Length);

	render::Texture* GetTexture();

	Glyph* GetGlyph(char);

	int GetHeight() const {return height_;}
};

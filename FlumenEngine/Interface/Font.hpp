#pragma once

#include "FlumenCore/Conventions.hpp"

class File;
class Texture;
class Glyph;

class Font
{
	int size_;

	float ascent_;

	float descent_;

	Texture* texture_;

	Array <Glyph> glyphs_;

public:
	Font();

	Font(File*, Length);

	Texture* GetTexture();

	Glyph* GetGlyph(char);
};

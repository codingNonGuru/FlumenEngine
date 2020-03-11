#include "Glyph.hpp"

Glyph::Glyph() {}

Glyph::Glyph(Float2 scale, float advance, Float2 position, Float2 textureOffset, Float2 textureScale)
{
	data_.Scale_ = scale;
	data_.Position_ = position;
	data_.TextureOffset_ = textureOffset;
	data_.TextureScale_ = textureScale;

	advance_ = advance;
}

GlyphData Glyph::GetData()
{
	return data_;
}

float Glyph::GetAdvance()
{
	return advance_;
}

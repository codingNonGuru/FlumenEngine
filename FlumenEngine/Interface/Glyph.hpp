#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenEngine/Utility/Color.hpp"

struct GlyphData
{
	Color Color_;

	Float2 Scale_;

	Float2 Position_;

	Float2 TextureOffset_;

	Float2 TextureScale_;

	int RowIndex_;

	int Padding_[3];
};

class Glyph
{
	GlyphData data_;

	float advance_;

public:
	float MinimumX_;

	Glyph();

	Glyph(Float2, float, Float2, Float2, Float2);

	GlyphData GetData();

	float GetAdvance();
};


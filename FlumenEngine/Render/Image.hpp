#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenEngine/Core/Types.hpp"

class DataBuffer;
class Color;

class Image
{
	DataBuffer* buffer_;

	Size size_;

	ImageFormats format_;

public:
	Image();

	Image(Size, ImageFormats);

	Image(Size, Color&, ImageFormats);

	Image(Size, DataBuffer*, Size, Size);

	void Clear(Color&);

	void Setup(Size, Color&);

	void Bind(Index);

	Size GetSize() const;

	void SetSize(Size);

	void Apply(Image*, Float, Color&, Size);

	operator DataBuffer();
};

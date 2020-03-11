#pragma once

#include "FlumenCore/Conventions.hpp"

class DataBuffer;

class ImageProcessor
{
	static void InitializeBlurModule();

public:
	static DataBuffer* Blur(DataBuffer*, Size, int);

	static DataBuffer* GetOutputBuffer();
};

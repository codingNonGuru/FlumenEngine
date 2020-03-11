#pragma once

#include "FlumenCore/Conventions.hpp"

class Shader;
class DataBuffer;

typedef float ContrastStrength;
typedef float ContrastThreshold;
typedef float FocusIndex;

class Perlin
{
	static Shader* shader_;

	static Map <DataBuffer> buffers_;

	static DataBuffer* targetBuffer_;

	static bool isInitialized_;

	static void Initialize();

public:
	static void Destroy();

	static void SetTargetBuffer(DataBuffer*);

	static DataBuffer* Generate(Size, FocusIndex, ContrastThreshold = 0.5f, ContrastStrength = 1.0f, Range = Range(0.0f, 1.0f));

	static void Download(container::Matrix*);

	static DataBuffer* GetResultBuffer();
};

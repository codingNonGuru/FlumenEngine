#pragma once

#include "FlumenCore/Conventions.hpp"

struct GlyphData;
class DataBuffer;
class Shader;

class TextManager
{
	static Array <GlyphData> glyphDataQueue_;

	static DataBuffer* glyphBuffer_;

	static Shader* shader_;

public:
	static void Initialize(Shader*);

	static Array <GlyphData> & GetDataQueue();

	static DataBuffer * GetBuffer();

	static Shader * GetShader();
};

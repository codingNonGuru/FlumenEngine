#include "FlumenEngine/Interface/TextManager.hpp"

#include "FlumenEngine/Interface/Glyph.hpp"
#include "FlumenEngine/Render/DataBuffer.hpp"

#define MAXIMUM_GLYPH_COUNT 1024

Array <GlyphData> TextManager::glyphDataQueue_ = Array <GlyphData> ();

DataBuffer* TextManager::glyphBuffer_ = nullptr;

Shader* TextManager::shader_ = nullptr;

void TextManager::Initialize(Shader* shader)
{
	shader_ = shader;

	glyphDataQueue_ = Array <GlyphData> (MAXIMUM_GLYPH_COUNT);

	glyphBuffer_ = new DataBuffer(glyphDataQueue_.GetMemoryCapacity());
}

Array <GlyphData> & TextManager::GetDataQueue()
{
	return glyphDataQueue_;
}

DataBuffer * TextManager::GetBuffer()
{
	return glyphBuffer_;
}

Shader* TextManager::GetShader()
{
	return shader_;
}

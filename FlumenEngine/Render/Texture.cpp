#include <stdlib.h>
#include <regex>

#include "GL/glew.h"
#include "SDL2/SDL_image.h"

#include "FlumenEngine/Render/Texture.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Core/File.hpp"
#include "FlumenEngine/Render/DataBuffer.hpp"

void * reusableData = new Byte[4096 * 4096 * 16];

using namespace render;

Texture::Texture()
{
	key_ = 0;
}

Texture::Texture(Size size, TextureFormats format, DataBuffer & buffer)
{
	key_ = 0;

	size_ = size;

	format_ = format;

	Length memorySize;
	switch(format_)
	{
	case TextureFormats::FOUR_BYTE:
		memorySize = 4;
		break;
	case TextureFormats::FOUR_FLOAT:
		memorySize = 16;
		break;
	case TextureFormats::THREE_BYTE:
		memorySize = 3;
		break;
	case TextureFormats::THREE_FLOAT:
		memorySize = 12;
		break;
	case TextureFormats::ONE_BYTE:
		memorySize = 1;
		break;
	case TextureFormats::ONE_FLOAT:
		memorySize = 4;
		break;
	case TextureFormats::ONE_INTEGER:
		memorySize = 4;
		break;
	}

	buffer.Download(reusableData, memorySize * size.x * size.y);
	Upload(reusableData);
}

Texture::Texture(Size size, TextureFormats format, container::Matrix* grid = nullptr)
{
	key_ = 0;

	size_ = size;

	format_ = format;

	void* data = grid ? grid->GetData() : nullptr;
	Upload(data);
}

Texture::Texture(File* file)
{
	key_ = 0;

	Initialize(file);
}

void Texture::Initialize(File* file)
{
	file_ = file;

	const char* fileName = file->GetName();
	std::regex expression(".*\.png");
	std::cmatch match;
	bool isMatched = std::regex_match(fileName, match, expression);

	if(isMatched)
	{
		std::cout<<"Texture "<<fileName<<" is of PNG (external) format.\n";
		ProcessPngFormat();
	}
	else
	{
		std::cout<<"Texture "<<fileName<<" is of TEX (internal) format.\n";
		ProcessInternalFormat();
	}
}

Texture::~Texture()
{

}

void Texture::ProcessInternalFormat()
{
	FILE* fileStream = fopen(file_->GetPath(), "rb");
	if(fileStream == nullptr)
		return;

	fread(&size_, sizeof(size_), 1, fileStream);
	fread(&format_, sizeof(format_), 1, fileStream);

	container::Matrix * grid = nullptr;

	switch(format_)
	{
	case TextureFormats::FOUR_BYTE:
		grid = new Grid <Byte4> (size_.x, size_.y);
		break;
	case TextureFormats::FOUR_FLOAT:
		grid = new Grid <Float4> (size_.x, size_.y);
		break;
	case TextureFormats::THREE_BYTE:
		grid = new Grid <Byte3> (size_.x, size_.y);
		break;
	case TextureFormats::THREE_FLOAT:
		grid = new Grid <Float3> (size_.x, size_.y);
		break;
	case TextureFormats::ONE_BYTE:
		grid = new Grid <Byte> (size_.x, size_.y);
		break;
	case TextureFormats::ONE_FLOAT:
		grid = new Grid <Float> (size_.x, size_.y);
		break;
	case TextureFormats::ONE_INTEGER:
		grid = new Grid <Integer> (size_.x, size_.y);
		break;
	}

	fread(grid->GetData(), grid->GetMemorySize(), 1, fileStream);

	fclose(fileStream);

	Upload(grid->GetData());
}

void Texture::ProcessPngFormat()
{
	SDL_Surface* swordImage = IMG_Load(file_->GetPath());

	size_.x = swordImage->w;
	size_.y = swordImage->h;

	format_ = TextureFormats::FOUR_BYTE;

	Grid<Byte4> texture (swordImage->w, swordImage->h);

	for(int x = 0; x < swordImage->w; ++x)
		for(int y = 0; y < swordImage->h; ++y)
		{
			auto pixel = texture(x, y);
			auto rawPixel = (Byte4*)swordImage->pixels + swordImage->w * y + x;
			*pixel = *rawPixel;
		}

	SDL_FreeSurface(swordImage);

	Upload(texture.GetData());
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, key_);

	DEBUG_OPENGL
}

void Texture::Bind(GLuint index, Shader* shader, const char* name)
{
	GLuint location = shader->GetTextureLocation(name);
	glUniform1i(location, index);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, key_);

	DEBUG_OPENGL
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);

	DEBUG_OPENGL
}

void Texture::Upload(void* data)
{
	if(key_ == 0)
	{
		glGenTextures(1, &key_);
		glBindTexture(GL_TEXTURE_2D, key_);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		switch(format_)
		{
		case TextureFormats::FOUR_BYTE:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size_.x, size_.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		case TextureFormats::FOUR_FLOAT:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size_.x, size_.y, 0, GL_RGBA, GL_FLOAT, data);
			break;
		case TextureFormats::THREE_BYTE:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size_.x, size_.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case TextureFormats::THREE_FLOAT:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, size_.x, size_.y, 0, GL_RGB, GL_FLOAT, data);
			break;
		case TextureFormats::ONE_BYTE:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, size_.x, size_.y, 0, GL_RED, GL_UNSIGNED_BYTE, data);
			break;
		case TextureFormats::ONE_FLOAT:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size_.x, size_.y, 0, GL_RED, GL_FLOAT, data);
			break;
		case TextureFormats::ONE_INTEGER:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, size_.x, size_.y, 0, GL_RED_INTEGER, GL_INT, data);
			break;
		case TextureFormats::DEPTH_FLOAT:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size_.x, size_.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
			break;
		case TextureFormats::DEPTH_BYTE:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size_.x, size_.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
			break;
		}

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, key_);

		switch(format_)
		{
		case TextureFormats::FOUR_BYTE:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size_.x, size_.y, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		case TextureFormats::FOUR_FLOAT:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size_.x, size_.y, GL_RGBA, GL_FLOAT, data);
			break;
		case TextureFormats::THREE_BYTE:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size_.x, size_.y, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case TextureFormats::THREE_FLOAT:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size_.x, size_.y, GL_RGB, GL_FLOAT, data);
			break;
		case TextureFormats::ONE_BYTE:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size_.x, size_.y, GL_RED, GL_UNSIGNED_BYTE, data);
			break;
		case TextureFormats::ONE_FLOAT:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size_.x, size_.y, GL_RED, GL_FLOAT, data);
			break;
		case TextureFormats::ONE_INTEGER:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size_.x, size_.y, GL_RED_INTEGER, GL_INT, data);
			break;
		case TextureFormats::DEPTH_FLOAT:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size_.x, size_.y, GL_DEPTH_COMPONENT, GL_FLOAT, data);
			break;
		case TextureFormats::DEPTH_BYTE:
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size_.x, size_.y, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
			break;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	DEBUG_OPENGL
}

void Texture::DownloadBackBuffer()
{
	Bind();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glReadBuffer(GL_BACK);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, size_.x, size_.y, 0);
	Unbind();
}

void Texture::Delete()
{
	glBindTexture(GL_TEXTURE_2D, key_);
	glDeleteTextures(1, &key_);

	DEBUG_OPENGL
}


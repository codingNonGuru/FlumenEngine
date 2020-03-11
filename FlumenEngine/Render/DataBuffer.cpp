#include <iostream>

#include <GL/glew.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "FlumenEngine/Render/DataBuffer.hpp"

#include "FlumenCore/Conventions.hpp"

DataBuffer::DataBuffer(int memorySize, void* data)
{
	Generate(memorySize, data);
}

void DataBuffer::Generate(int memorySize, void* data)
{
	type_ = GL_SHADER_STORAGE_BUFFER;

	memorySize_ = memorySize;

	glGenBuffers(1, &key_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, key_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, memorySize, data, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	DEBUG_OPENGL
}

void DataBuffer::UploadData(void* data, GLuint size)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, key_);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	DEBUG_OPENGL
}

void DataBuffer::Download(Container * container)
{
	auto data = container->GetData();
	if(data == nullptr)
	{
		std::cout<<"Container Data is not initialized.\n";
		return;
	}

	Bind();

	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, container->GetMemorySize(), data);

	Unbind();
}

void DataBuffer::Download(void * data, Length size)
{
	if(data == nullptr)
	{
		std::cout<<"Buffer Data is not initialized.\n";
		return;
	}

	Bind();

	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);

	Unbind();
}

void DataBuffer::Copy(DataBuffer* destination)
{
	glBindBuffer(GL_COPY_READ_BUFFER, key_);
	glBindBuffer(GL_COPY_WRITE_BUFFER, destination->key_);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, memorySize_);

	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

void DataBuffer::Bind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, key_);

	DEBUG_OPENGL
}

void DataBuffer::Bind(GLuint bindPoint)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, key_);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindPoint, key_);

	DEBUG_OPENGL
}

void DataBuffer::Unbind()
{
	glBindBuffer(type_, 0);

	DEBUG_OPENGL
}

void DataBuffer::Delete()
{
	Bind();

	glDeleteBuffers(1, &key_);

	Unbind();
}



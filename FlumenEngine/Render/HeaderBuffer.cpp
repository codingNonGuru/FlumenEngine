#include <GL/glew.h>

#include "HeaderBuffer.hpp"

#include "FlumenEngine/Render/DataBuffer.hpp"

HeaderBuffer::HeaderBuffer(int count)
{
	if(count != 0)
	{
		slaveBuffers_.Initialize(count);
	}

	Generate();
}

void HeaderBuffer::Generate()
{
	glGenVertexArrays(1, &key_);

	DEBUG_OPENGL
}

void HeaderBuffer::AddStorageBuffer(int index, DataBuffer* buffer, const char* bufferName)
{
	if(!buffer)
		return;

	if(buffer->GetType() != GL_SHADER_STORAGE_BUFFER)
	{
		std::cout<<"Buffer is not of Storage type.\n";
		return;
	}

	auto bufferPointer = slaveBuffers_.Add(bufferName);
	*bufferPointer = buffer;

	Bind();
	buffer->Bind();
	glEnableVertexAttribArray(index);
	Unbind();
	buffer->Unbind();
}

void HeaderBuffer::AddBuffer(int index, DataBuffer* buffer, const char* name)
{
	auto bufferPointer = slaveBuffers_.Add(name);
	if(bufferPointer == nullptr)
	{
		std::cout<<"Allocating buffer "<<name<<" failed!\n";
		return;
	}

	*bufferPointer = buffer;

	Bind();
	buffer->Bind();
	Unbind();
	buffer->Unbind();
}

void HeaderBuffer::AddBuffer(int index, DataBuffer* buffer, const char* bufferName, GLuint elementSize, bool isFloating)
{

	if(!buffer)
		return;

	if(buffer->GetType() == GL_SHADER_STORAGE_BUFFER)
	{
		std::cout<<"Buffer is of Storage type.\n";
		return;
	}

	auto bufferPointer = slaveBuffers_.Add(bufferName);
	*bufferPointer = buffer;

	Bind();
	buffer->Bind();

	glEnableVertexAttribArray(index);
	if(isFloating)
	{
		glVertexAttribPointer(index, elementSize, GL_FLOAT, GL_FALSE, 0, NULL);
	}
	else
	{
		glVertexAttribIPointer(index, elementSize, GL_INT, 0, NULL);
	}

	Unbind();
	buffer->Unbind();
}

void HeaderBuffer::AddElementBuffer(DataBuffer* buffer)
{

	if(!buffer)
		return;

	if(buffer->GetType() != GL_ELEMENT_ARRAY_BUFFER)
	{
		std::cout<<"Buffer is not of Element type.\n";
		return;
	}

	auto bufferPointer = slaveBuffers_.Add("element");
	*bufferPointer = buffer;

	Bind();
	buffer->Bind();
	Unbind();
	buffer->Unbind();
}

DataBuffer* HeaderBuffer::GetBuffer(const char* bufferName)
{
	return *slaveBuffers_.Get(bufferName);
}

void HeaderBuffer::UploadData(const char* bufferName, void* data, GLuint memorySize)
{
	auto buffer = GetBuffer(bufferName);
	if(buffer != nullptr)
	{
		buffer->UploadData(data, memorySize);
	}
}

void HeaderBuffer::SetSlaveBindPoint(const char* bufferName, GLuint bindPoint)
{
	auto buffer = GetBuffer(bufferName);
	buffer->Bind(bindPoint);
}

void HeaderBuffer::Bind()
{
	glBindVertexArray(key_);

	DEBUG_OPENGL
}

void HeaderBuffer::Unbind()
{
	glBindVertexArray(0);

	DEBUG_OPENGL
}

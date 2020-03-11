#pragma once

#include "FlumenCore/Conventions.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class DataBuffer
{
	GLuint key_;

	GLenum type_;

	unsigned int size_;

	unsigned int memorySize_;

public:
	DataBuffer() {}

	DataBuffer(int, void* = nullptr);

	GLenum GetType() const {return type_;}

	void Generate(int, void* = nullptr);

	void UploadData(void*, GLuint);

	void Download(Container *);

	void Download(void *, Length);

	void Copy(DataBuffer*);

	void Bind();

	void Bind(GLuint);

	void Unbind();

	void SetSize(unsigned int size) {size_ = size;}

	unsigned int GetSize() const {return size_;}

	unsigned int GetMemorySize() const {return memorySize_;}

	void Delete();
};

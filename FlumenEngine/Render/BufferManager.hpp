#pragma once

#include "FlumenCore/Conventions.hpp"

class HeaderBuffer;
class DataBuffer;
class FrameBuffer;

class BufferManager
{
private:
	static Map <HeaderBuffer*> headerBuffers_;

	static Map <DataBuffer*> dataBuffers_;

	static Map <FrameBuffer*> frameBuffers_;

public:
	static Map <HeaderBuffer*> & GetHeaderBuffers();

	static Map <DataBuffer*> & GetDataBuffers();

	static Map <FrameBuffer*> & GetFrameBuffers();

	static HeaderBuffer* GetHeaderBuffer(const char*);

	static DataBuffer* GetDataBuffer(const char*);

	static FrameBuffer* GetFrameBuffer(const char*);

	static void BindFrameBuffer(Word);
};

#include "BufferManager.hpp"

#include "HeaderBuffer.hpp"
#include "FlumenEngine/Render/DataBuffer.hpp"
#include "FrameBuffer.hpp"

#define MAXIMUM_HEADER_BUFFER_COUNT 128

#define MAXIMUM_DATA_BUFFER_COUNT 1024

#define MAXIMUM_FRAME_BUFFER_COUNT 32

Map <HeaderBuffer*> BufferManager::headerBuffers_ = Map <HeaderBuffer*> (MAXIMUM_HEADER_BUFFER_COUNT);

Map <DataBuffer*> BufferManager::dataBuffers_ = Map <DataBuffer*> (MAXIMUM_DATA_BUFFER_COUNT);

Map <FrameBuffer*> BufferManager::frameBuffers_ = Map <FrameBuffer*> (MAXIMUM_FRAME_BUFFER_COUNT);

Map <HeaderBuffer*> & BufferManager::GetHeaderBuffers()
{
	return headerBuffers_;
}

Map <DataBuffer*> & BufferManager::GetDataBuffers()
{
	return dataBuffers_;
}

Map <FrameBuffer*> & BufferManager::GetFrameBuffers()
{
	return frameBuffers_;
}

HeaderBuffer* BufferManager::GetHeaderBuffer(const char* name)
{
	auto buffer = headerBuffers_.Get(name);
	if(buffer == nullptr)
		return nullptr;

	return *buffer;
}

DataBuffer* BufferManager::GetDataBuffer(const char* name)
{
	auto buffer = dataBuffers_.Get(name);
	if(buffer == nullptr)
		return nullptr;

	return *buffer;
}

FrameBuffer* BufferManager::GetFrameBuffer(const char* name)
{
	auto buffer = frameBuffers_.Get(name);
	if(buffer == nullptr)
		return nullptr;

	return *buffer;
}

void BufferManager::BindFrameBuffer(Word name)
{
	auto buffer = frameBuffers_.Get(name);
	if(buffer == nullptr)
		return;

	(*buffer)->BindBuffer();
}

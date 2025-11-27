#pragma once

#include <mutex>

#include "FlumenCore/Container/Pool.hpp"
#include "FlumenCore/Container/Array.hpp"
#include "FlumenCore/Container/Map.hpp"
#include "FlumenCore/Singleton.h"

#include "FlumenEngine/Thread/ThreadManager.h"
#include "FlumenEngine/Thread/HexBuffer.h"

namespace engine::thread
{
    template <typename HexType>
    class HexBufferManager : public core::Singleton <HexBufferManager <HexType>>
    {
        friend class core::Singleton <HexBufferManager>;

        HexBufferBatch <HexType> bufferBatches[engine::ThreadManager::GetThreadCount() + 2];

        container::StaticMap <HexBufferBatch <HexType> *, std::thread::id> bufferBatchMap {engine::ThreadManager::GetThreadCount() + 2};

        int lastBatchIndex {0};

    public:
        HexBuffer <HexType> *GetUsableBuffer()
        {
            const auto threadId = std::this_thread::get_id();

            auto batchPointer = bufferBatchMap.Get(threadId);
            if(batchPointer != nullptr)
            {
                auto batch = *batchPointer;

                auto buffer = batch->Buffers.Add();
                buffer->Tiles.Reset();

                return buffer;
            }

            std::mutex mutex;
            mutex.lock();

            batchPointer = bufferBatchMap.Add(threadId);

            *batchPointer = &bufferBatches[lastBatchIndex];
            lastBatchIndex++;

            mutex.unlock();

            auto batch = *batchPointer;
                
            auto buffer = batch->Buffers.Add();
            buffer->Tiles.Reset();

            return buffer;
        }
    };
}
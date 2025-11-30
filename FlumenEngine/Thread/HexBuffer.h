#pragma once

#include "FlumenCore/Container/Pool.hpp"
#include "FlumenCore/Container/Array.hpp"

namespace engine::thread
{
    #define TILES_PER_BUFFER 1024

    #define TILE_BUFFERS_PER_THREAD 8

    template <typename HexType>
    struct HexBuffer;

    template <typename HexType>
    struct HexBufferBatch
    {
        container::Pool <HexBuffer <HexType>> Buffers;

        HexBufferBatch() : Buffers(TILE_BUFFERS_PER_THREAD)
        {
            for(int i = 0; i < Buffers.GetCapacity(); ++i)
            {
                auto buffer = Buffers.Add();

                buffer->Initialize(this);
            }

            Buffers.Reset();
        }
    };

    template <typename HexType>        
    struct HexBuffer
    {
        container::Array <HexType *> Tiles;

        HexBufferBatch <HexType> *Batch {nullptr};

        HexBuffer() : Tiles(TILES_PER_BUFFER) {}

        void Initialize(HexBufferBatch <HexType> *batch) {Batch = batch;}

        ~HexBuffer()
        {
            //std::cout<<this<<" "<<Batch<<" "<< Tiles.GetStart() <<" "<<Batch->Buffers.GetSize()<<"\n";
            Batch->Buffers.Remove(Tiles.GetStart());
            //std::cout<<this<<" "<<Batch<<" "<< Tiles.GetStart() <<" "<<Batch->Buffers.GetSize()<<"\n\n";
        }

        HexBuffer(const HexBuffer &) = delete;

        HexBuffer& operator=(HexBuffer &buffer) = delete;

        HexBuffer(HexBuffer &&buffer) {Tiles = buffer.Tiles; Batch = buffer.Batch;}

        HexBuffer& operator=(HexBuffer &&buffer) {Tiles = buffer.Tiles; Batch = buffer.Batch; return *this;}

        bool operator== (HexType **tileStart) {return Tiles.GetStart() == tileStart;}

        int GetSize() const {return Tiles.GetSize();}

        auto begin() {return typename container::Array <HexType *>::template Iterator <HexType *>(Tiles, Tiles.GetStart());}

        auto end() {return typename container::Array <HexType *>::template Iterator <HexType *>(Tiles, Tiles.GetEnd());}
    };
}
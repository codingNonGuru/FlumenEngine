#pragma once

#include <concepts>

#include "FlumenCore/Conventions.hpp"
#include "FlumenCore/Hex/Hex.h"
#include "FlumenCore/Singleton.h"

#include "FlumenEngine/Thread/HexBufferManager.h"

namespace container
{
    template <std::derived_from <core::hex::Tile> TileType>
    class HexGrid
    {
    protected:
        class NearbyBuffer : public core::Singleton <NearbyBuffer>
        {
            friend class core::Singleton <NearbyBuffer>;

            friend class HexGrid;

            container::Array <TileType *> tiles;

            NearbyBuffer()
            {
                tiles.Initialize(4096);
            }
        };

        container::Grid <TileType> tiles;

    public:
        HexGrid() {}

        HexGrid(int width, int height)
        {
            tiles.Initialize(width, height);
        }

        void Initialize(int width, int height, container::Grid <TileType>::Memory &tileMemory)
        {
            //nearbyTiles.Initialize(width * height, neighbourMemory);

            tiles.Initialize(width, height, tileMemory);
        }

        void Initialize(int width, int height)
        {
            //nearbyTiles.Initialize(width * height, neighbourMemory);

            tiles.Initialize(width, height);
        }

        TileType *Get(int x, int y)
        {
            return tiles.Get(x, y);
        }

        TileType *GetCenterTile()
        {
            return tiles.Get(tiles.GetWidth() / 2, tiles.GetHeight() / 2);
        }

        TileType *GetTile(Integer3 position)
        {
            return tiles.Get(position.x + position.z / 2, position.z);
        }

        TileType* GetTile(Integer2 position)
        {
            return tiles.Get(position.x, position.y);
        }

        container::Grid <TileType> &GetTiles()
        {
            return tiles;
        }

        const int GetSize() const 
        {
            return tiles.GetWidth();
        }

        const int GetTileCount() const 
        {
            return tiles.GetWidth() * tiles.GetHeight();
        }

        TileType* GetRandomTile()
        {
            auto i = utility::GetRandom(0, tiles.GetWidth() - 1);
            auto j = utility::GetRandom(0, tiles.GetHeight() - 1);

            return tiles.Get(i, j);
        }

        const engine::thread::HexBuffer <TileType> GetNearbyTiles(TileType *tile, Integer range)
        {
            static const auto bufferManager = engine::thread::HexBufferManager <TileType>::Get();
            auto buffer = bufferManager->GetUsableBuffer();

            for(Integer x = -range; x <= range; ++x)
            {
                for(Integer y = -range; y <= range; ++y)
                {
                    for(Integer z = -range; z <= range; ++z)
                    {
                        if(x + y + z == 0)
                        {
                            auto nearbyTile = GetTile(tile->Coordinates + Integer3(x, y, z));
                            if(nearbyTile != nullptr)
                            {
                                *buffer->Tiles.Add() = nearbyTile;
                            }
                        }
                    }
                }
            }

            return std::move(*buffer);
        }

        container::Block <TileType *, 6> GetNearbyTiles(TileType* tile)
        {
            auto nearbyTiles = container::Block <TileType *, 6> (nullptr);

            int i = 0;
            for(Integer x = -1; x <= 1; ++x)
            {
                for(Integer y = -1; y <= 1; ++y)
                {
                    for(Integer z = -1; z <= 1; ++z)
                    {
                        if(x + y + z == 0)
                        {
                            auto nearbyTile = GetTile(tile->Coordinates + Integer3(x, y, z));
                            if(nearbyTile != tile)
                            {
                                *nearbyTiles[i] = nearbyTile;
                                i++;
                            }
                        }
                    }
                }
            }

            return nearbyTiles;
        }

        const engine::thread::HexBuffer <TileType> GetTileRing(TileType *tile, Integer range)
        {
            static const auto bufferManager = engine::thread::HexBufferManager <TileType>::Get();
            auto buffer = bufferManager->GetUsableBuffer();

            for(Integer x = -range; x <= range; ++x)
            {
                for(Integer y = -range; y <= range; ++y)
                {
                    for(Integer z = -range; z <= range; ++z)
                    {
                        if(x + y + z == 0 && abs(x) + abs(y) + abs(z) == range * 2)
                        {
                            auto nearbyTile = GetTile(tile->Coordinates + Integer3(x, y, z));
                            if(nearbyTile != nullptr)
                            {
                                *buffer->Tiles.Add() = nearbyTile;
                            }
                        }
                    }
                }
            }

            return std::move(*buffer);
        }
    };

    
}
#pragma once

#include <concepts>

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Container/HexGrid.h"

namespace container
{
    template <
        std::derived_from <core::hex::Tile> TileType, 
        std::derived_from <core::hex::HexEdge <TileType>> EdgeType
        >
    class EdgyHexGrid : public HexGrid <TileType>
    {
        container::Grid <EdgeType> edges;

    public:
        EdgyHexGrid() : HexGrid <TileType> () {}
        
        EdgyHexGrid(int width, int height) : HexGrid <TileType> (width, height)
        {
            AddEdges();
        }

        void AddEdges()
        {
            edges.Initialize(this->tiles.GetWidth() * 3, this->tiles.GetHeight());
        }

        void AddEdges(container::Grid <EdgeType>::Memory &memory)
        {
            edges.Initialize(this->tiles.GetWidth() * 3, this->tiles.GetHeight(), memory);
        }

        EdgeType *GetEdge(int x, int y)
        {
            return edges.Get(x, y);
        }

        Integer2 GetEdgeCoordinates(TileType *first, TileType *second)
        {
            auto firstPosition = first->GetSquarePosition();
            auto secondPosition = second->GetSquarePosition();

            if(firstPosition.y == secondPosition.y)
            {
                auto x = firstPosition.x < secondPosition.x ? firstPosition.x : secondPosition.x;
                auto y = firstPosition.y;

                return {x, y};
            }
            else
            {
                int y = firstPosition.y < secondPosition.y ? firstPosition.y : secondPosition.y;
                auto x = this->tiles.GetWidth() + firstPosition.x + secondPosition.x + 1;

                return {x, y};
            }
        }

        EdgeType *GetEdge(TileType *first, TileType *second)
        {
            auto firstPosition = first->GetSquarePosition();
            auto secondPosition = second->GetSquarePosition();

            if(firstPosition.y == secondPosition.y)
            {
                auto x = firstPosition.x < secondPosition.x ? firstPosition.x : secondPosition.x;
                auto y = firstPosition.y;

                return edges.Get(x, y);
            }
            else
            {
                int y = firstPosition.y < secondPosition.y ? firstPosition.y : secondPosition.y;

                auto x = this->tiles.GetWidth() + firstPosition.x + secondPosition.x + 1;

                return edges.Get(x, y);
            }
        }

        container::Grid <EdgeType> &GetEdges() {return edges;}
    };
}
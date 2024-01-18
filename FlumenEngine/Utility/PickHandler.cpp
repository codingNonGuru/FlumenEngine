#include "FlumenCore/Utility/Utility.hpp"

#include "FlumenEngine/Utility/PickHandler.h"
#include "FlumenEngine/Render/Mesh.hpp"
#include "FlumenEngine/Render/Camera.hpp"
#include "FlumenEngine/Core/InputHandler.hpp"

bool PickHandler::CheckCollision(Camera * camera, Mesh * mesh, Float3 meshOffset, Float meshScale)
{
    Position2 mousePosition = InputHandler::GetMousePosition();

    auto positionAttribute = mesh->GetAttribute("position");
    auto positionData = (Array<Float3> *)positionAttribute->GetData();

    auto indexAttribute = mesh->GetAttribute("index");
    auto indexData = (Array<Index> *)indexAttribute->GetData();

    for(Index i = 0; i < indexData->GetSize(); i += 3)
    {
        auto firstIndex = *indexData->Get(i);
        auto secondIndex = *indexData->Get(i + 1);
        auto thirdIndex = *indexData->Get(i + 2);

        auto firstVertex = *positionData->Get(firstIndex) * meshScale + meshOffset;
        auto secondVertex = *positionData->Get(secondIndex) * meshScale + meshOffset;
        auto thirdVertex = *positionData->Get(thirdIndex) * meshScale + meshOffset;

        Position2 screenPositions[] = {
            camera->GetScreenPosition(firstVertex),
            camera->GetScreenPosition(secondVertex),
            camera->GetScreenPosition(thirdVertex),
        };

        bool isInside = utility::IsInsideTriangle(mousePosition, screenPositions[0], screenPositions[1], screenPositions[2]);
        if(isInside)
        {
            return true;
        }
    }

    return false;
}
#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Core/Transform.hpp"

class LayoutGroup : public Element
{
    int elementsPerRow {4};

    float horizontalDistance {0.0f};

    float verticalDistance {0.0f};

    void HandleUpdate() override
    {
        auto width = 0.0f;
        auto rowWidth = 0.0f;
        auto height = 0.0f;
        Length activeElementCount = 0;
        for(auto iterator = children_.GetStart(); iterator != children_.GetEnd(); ++iterator)
        {
            auto element = *iterator;
            if(!element->IsGloballyActive())
                continue;

            width += element->GetSize().x;

            if(activeElementCount < elementsPerRow)
            {
                rowWidth += element->GetSize().x;
            }

            if(activeElementCount % elementsPerRow == 0)
            {
                height += element->GetSize().y;
            }

            activeElementCount++;
        }

        auto horizontalOffset = horizontalDistance * Float(elementsPerRow - 1) + rowWidth;
        horizontalOffset *= 0.5f;
        horizontalOffset -= (*children_.GetStart())->GetSize().x * 0.5f;

        auto verticalOffset = verticalDistance * Float((activeElementCount / elementsPerRow) - (activeElementCount % elementsPerRow == 0 ? 1 : 0)) + height;
        verticalOffset *= 0.5f;
        verticalOffset -= (*children_.GetStart())->GetSize().y * 0.5f;

        auto position = Position3(0.0f, -verticalOffset, 0.0f);

        auto index = 0;
        for(auto iterator = children_.GetStart(); iterator != children_.GetEnd(); ++iterator)
        {
            auto element = *iterator;
            if(!element->IsGloballyActive())
                continue;

            if(index % elementsPerRow == 0)
            {
                position.x = -horizontalOffset;

                bool hasOnlyOneRow = activeElementCount / elementsPerRow == 0;
                if(index != 0 && hasOnlyOneRow == false)
                {
                    position.y += verticalDistance;
                    position.y += (element->GetSize().y + (*(iterator - elementsPerRow))->GetSize().y) * 0.5f;
                }
            }

            element->GetTransform()->GetPosition() = position;

            auto nextLabel = *(iterator + 1);
            if(iterator == children_.GetEnd() - 1)
                break;

            position.x += (element->GetSize().x + nextLabel->GetSize().x) * 0.5f;
            position.x += horizontalDistance;

            index++;
        }

        size_.x = horizontalDistance * Float(elementsPerRow - 1) + rowWidth;
        size_.y = verticalDistance * Float((activeElementCount / elementsPerRow) - (activeElementCount % elementsPerRow == 0 ? 1 : 0)) + height;
        
        UpdatePosition();
    }

public:
    void SetDistancing(int elementsPerRow, float elementDistance)
    {
        this->elementsPerRow = elementsPerRow;
        this->horizontalDistance = elementDistance;
        this->verticalDistance = elementDistance;
    }

    void SetDistancing(int elementsPerRow, float horizontalDistance, float verticalDistance)
    {
        this->elementsPerRow = elementsPerRow;
        this->horizontalDistance = horizontalDistance;
        this->verticalDistance = verticalDistance;
    }
};
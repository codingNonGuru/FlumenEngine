#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Core/Transform.hpp"

class LayoutGroup : public Element
{
    int elementsPerRow {4};

    float horizontalDistance {0.0f};

    float verticalDistance {0.0f};

    bool isWidthLocked {false};

    bool isHeightLocked {false};

    Position2 generalOffset {};

    Size padding;

    void HandleUpdate() override
    {
        auto width = 0.0f;
        auto rowWidth = 0.0f;
        auto height = 0.0f;
        Length activeElementCount = 0;
        
        for(auto child : staticChildren_)
        {
            if(child->IsGloballyActive() == false)
                continue;

            width += child->GetSize().x;

            if(activeElementCount < elementsPerRow)
            {
                rowWidth += child->GetSize().x;
            }

            if(activeElementCount % elementsPerRow == 0)
            {
                height += child->GetSize().y;
            }

            activeElementCount++;
        }

        auto horizontalOffset = horizontalDistance * Float(elementsPerRow - 1) + rowWidth;
        horizontalOffset *= 0.5f;
        horizontalOffset -= (*staticChildren_.GetStart())->GetSize().x * 0.5f;

        auto verticalOffset = verticalDistance * Float((activeElementCount / elementsPerRow) - (activeElementCount % elementsPerRow == 0 ? 1 : 0)) + height;
        verticalOffset *= 0.5f;
        verticalOffset -= (*staticChildren_.GetStart())->GetSize().y * 0.5f;

        auto position = Position2(0.0f, -verticalOffset);

        auto index = 0;
        for(auto iterator = staticChildren_.GetStart(); iterator != staticChildren_.GetEnd(); ++iterator)
        {
            auto child = *iterator;
            if(child->IsGloballyActive() == false)
                continue;

            if(index % elementsPerRow == 0)
            {
                position.x = -horizontalOffset;

                bool hasOnlyOneRow = activeElementCount / elementsPerRow == 0;
                if(index != 0 && hasOnlyOneRow == false)
                {
                    position.y += verticalDistance;
                    position.y += (child->GetSize().y + (*(iterator - elementsPerRow))->GetSize().y) * 0.5f;
                }
            }

            child->SetBasePosition(position + generalOffset);

            auto nextLabel = *(iterator + 1);
            if(iterator == staticChildren_.GetEnd() - 1)
                break;

            position.x += (child->GetSize().x + nextLabel->GetSize().x) * 0.5f;
            position.x += horizontalDistance;

            index++;
        }

        if(isWidthLocked == false)
        {
            size_.x = horizontalDistance * Float(elementsPerRow - 1) + rowWidth;

            size_.x += padding.x;
        }

        if(isHeightLocked == false)
        {
            size_.y = verticalDistance * Float((activeElementCount / elementsPerRow) - (activeElementCount % elementsPerRow == 0 ? 1 : 0)) + height;

            size_.y += padding.y;
        }
        
        UpdatePosition();

        for(auto &child : dynamicChildren_)
        {
            child->UpdatePosition();
        }
    }

public:
    void SetDistancing(int elementsPerRow, float elementDistance = 0.0f)
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

    void LockWidth(int width)
    {
        isWidthLocked = true;

        GetSize().x = width;
    }

    void LockHeight(int height)
    {
        isHeightLocked = true;

        GetSize().y = height;
    }

    void SetOffset(Position2 offset)
    {
        generalOffset = offset;
    }

    void SetPadding(Size newPadding)
    {
        padding = newPadding;
    }
};
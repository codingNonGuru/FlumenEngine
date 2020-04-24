#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Core/Transform.hpp"

class LayoutGroup
{
    const Array <Element *> & elements;

    float elementDistance = 20.0f;

    Position3 basePosition;

public:
    LayoutGroup(const Array <Element *> & _elements, Position3 _basePosition) : elements(_elements), basePosition(_basePosition)
    {
    }

    void Refresh()
    {
        auto width = 0.0f;
        Length activeElementCount = 0;
        for(auto iterator = elements.GetStart(); iterator != elements.GetEnd(); ++iterator)
        {
            auto element = *iterator;
            if(!element->IsGloballyActive())
                continue;

            width += element->GetSize().x;

            activeElementCount++;
        }

        auto offset = Direction3(elementDistance, 0.0f, 0.0f);

        auto position = basePosition - offset * (Float(activeElementCount - 1) / 2.0f);
        auto firstElement = elements.GetStart();
        position.x += (*firstElement)->GetSize().x * 0.5f;
        position.x -= width * 0.5f;

        for(auto iterator = elements.GetStart(); iterator != elements.GetEnd(); ++iterator)
        {
            auto element = *iterator;
            if(!element->IsGloballyActive())
                continue;

            element->GetTransform()->GetPosition() = position;

            auto nextLabel = *(iterator + 1);
            position.x += (element->GetSize().x + nextLabel->GetSize().x) * 0.5f;
            position += offset;
        }
    }
};
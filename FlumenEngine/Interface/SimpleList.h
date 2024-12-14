#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Core/Transform.hpp"

class Scroller;

enum class ListOrientations
{
    HORIZONTAL,
    VERTICAL
};

class SimpleList : public Element
{
    ListOrientations orientation {ListOrientations::VERTICAL};

    float distance {0.0f};

    bool isWidthLocked {false};

    bool isHeightLocked {false};

    Position2 generalOffset {};

    Size padding;

    Scroller *scroller {nullptr};

    void HandleVeryLateUpdate() override
    {
        //Reset data
        int width = 0;
        int height = 0;

        auto activeElementIndex = 0;

        //Count active elements
        for(auto child : staticChildren_)
        {
            if(child->IsGloballyActive() == false)
                continue;

            activeElementIndex++;
        }

        //Calculate the width & height of the entire layout
        if(activeElementIndex == 1)
        {
            width = (*staticChildren_.GetStart())->GetWidth();

            height = (*staticChildren_.GetStart())->GetHeight();
        }
        else if(activeElementIndex > 1)
        {
            if(orientation == ListOrientations::HORIZONTAL)
            {
                width = (*staticChildren_.GetStart())->GetWidth() * activeElementIndex + distance * (activeElementIndex - 1);

                height = (*staticChildren_.GetStart())->GetHeight();
            }
            else
            {
                width = (*staticChildren_.GetStart())->GetWidth();

                height = (*staticChildren_.GetStart())->GetHeight() * activeElementIndex + distance * (activeElementIndex - 1);
            }
        }

        float x = generalOffset.x;
        float y = generalOffset.y;

        //Calculate element positions
        for(auto child : staticChildren_)
        {
            if(child->IsGloballyActive() == false)
                continue;
   
            child->SetAnchorData(ElementAnchors::UPPER_LEFT, ElementPivots::UPPER_LEFT);

            child->SetBasePosition(Position2(x, y));

            if(orientation == ListOrientations::HORIZONTAL)
            {
                x += child->GetWidth() + distance;
            }
            else
            {
                y += child->GetHeight() + distance;
            }
        }

        //Set layout size
        if(isWidthLocked == false)
        {
            SetWidth(width + padding.x * 2);
        }

        if(isHeightLocked == false)
        {
            SetHeight(height + padding.y * 2);
        }

        //Update layout position & dynamic children positions
        UpdatePosition();

        for(auto &child : dynamicChildren_)
        {
            child->UpdatePosition();
        }
    }

public:
    void Setup(ListOrientations orientation = ListOrientations::VERTICAL, float elementDistance = 0.0f)
    {
        this->distance = elementDistance;

        this->orientation = orientation;
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

    void MakeScrollable(int, int);

    void SetScrollableChildCount(int);
};

#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Core/Transform.hpp"

class Scroller;

class LayoutGroup : public Element
{
    struct Row
    {
        container::Array <Element *> Elements;

        int Width {0};

        int Height {0};

        float X;

        float Y;
    };

    container::Array <Row> rows;

    int elementsPerRow {4};

    float horizontalDistance {0.0f};

    float verticalDistance {0.0f};

    bool isWidthLocked {false};

    bool isHeightLocked {false};

    Position2 generalOffset {};

    Size padding;

    Scroller *scroller {nullptr};

    bool isAlignedLeft {true};

    void HandleConfigure() override
    {
        rows.Initialize(staticChildren_.GetCapacity());

        for(int i = 0; i < rows.GetCapacity(); ++i)
        {
            *rows.Allocate() = {staticChildren_.GetCapacity()};
        }
    }

    void HandleUpdate() override
    {
        //Reset data
        int width = 0;
        int height = 0;

        rows.Reset();

        auto activeElementIndex = 0;

        auto currentRow = rows.Add();
        currentRow->Elements.Reset();
        currentRow->Width = 0;
        currentRow->Height = 0;
        currentRow->X = 0.0f;
        currentRow->Y = 0.0f;

        //Fill rows with elements
        //Add element width to each row
        //Determine height for each row
        for(auto child : staticChildren_)
        {
            if(child->IsGloballyActive() == false)
                continue;

            *currentRow->Elements.Add() = child;

            if(child->GetHeight() > currentRow->Height)
            {
                currentRow->Height = child->GetHeight();
            }

            currentRow->Width += child->GetWidth();

            activeElementIndex++;

            if(activeElementIndex % elementsPerRow == 0)
            {
                currentRow = rows.Add();

                currentRow->Elements.Reset();
                currentRow->Width = 0;
                currentRow->Height = 0;
                currentRow->X = 0.0f;
                currentRow->Y = 0.0f;
            }
        }

        //Add horizontal spacing to the width of each row
        for(auto &row : rows)
        {
            if(row.Elements.GetSize() > 1)
            {
                row.Width += horizontalDistance * (row.Elements.GetSize() - 1);
            }
        }

        //Calculate the width & height of the entire layout
        if(rows.GetSize() == 1)
        {
            width = rows.GetStart()->Width;
            height = rows.GetStart()->Height;
        }
        else if(rows.GetSize() > 1)
        {
            for(auto &row : rows)
            {
                if(row.Width > width)
                {
                    width = row.Width;
                }

                height += row.Height;
            }

            height += verticalDistance * (rows.GetSize() - 1);
        }

        //Calculate row positions
        float x = 0.0f;
        float y = 0.0f;

        for(auto &row : rows)
        {
            row.X = x + padding.x;
            if(isAlignedLeft == false)
            {
                row.X += (width - row.Width) / 2;
            }

            row.Y = y + padding.y;

            y += row.Height + verticalDistance;
        }

        //Calculate element positions
        for(auto &row : rows)
        {
            float x = row.X + generalOffset.x;
            float y = row.Y + generalOffset.y;

            for(auto &element : row.Elements)
            {   
                element->SetAnchorData(ElementAnchors::UPPER_LEFT, ElementPivots::UPPER_LEFT);

                element->SetBasePosition(Position2(x, y));

                x += element->GetWidth() + horizontalDistance;
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

    /*void HandleUpdate() override
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

        int horizontalEffectiveCount = activeElementCount < elementsPerRow ? activeElementCount : elementsPerRow;

        auto horizontalOffset = horizontalDistance * Float(horizontalEffectiveCount - 1) + rowWidth;
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
            size_.x = horizontalDistance * Float(horizontalEffectiveCount - 1) + rowWidth;

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
    }*/

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

    void MakeScrollable(int, int);

    void SetScrollableChildCount(int);

    void AlignToCenter() {isAlignedLeft = false;}

    void AlignToLeft() {isAlignedLeft = true;}
};

#include "Scroller.h"
#include "FlumenEngine/Core/InputHandler.hpp"
#include "FlumenEngine/Interface/LayoutGroup.h"

void Scroller::Setup(LayoutGroup *parent, int newListSize, int activeChildCount)
{
    listSize = newListSize;

    firstElementIndex = 0;

    this->activeChildCount = activeChildCount;

    SetDynamicParent(parent);
}

void Scroller::SetActiveChildCount(int activeChildCount)
{
    this->activeChildCount = activeChildCount;
}

void Scroller::HandleUpdate()
{
    const auto parent = (Element *)parent_;

    if(listSize < activeChildCount)
    {
        auto &children = parent->GetChildren();

        for(int i = 0; i < activeChildCount; ++i)
        {
            auto child = *children.Get(i);

            if(i >= firstElementIndex && i < firstElementIndex + listSize)
            {
                child->Enable();
            }
            else
            {
                child->Disable();
            }
        }
    }

    SetPosition(parent->GetPosition());

    SetWidth(parent->GetSize().x);

    SetHeight(parent->GetSize().y);
}

void Scroller::HandleEnable()
{
    SetInteractivity(true);

    InputHandler::RegisterScrollUpEvent({this, &Scroller::HandleScrollUp});

    InputHandler::RegisterScrollDownEvent({this, &Scroller::HandleScrollDown});
}

void Scroller::HandleDisable()
{
    SetInteractivity(false);

    InputHandler::UnregisterScrollUpEvent({this, &Scroller::HandleScrollUp});

    InputHandler::UnregisterScrollDownEvent({this, &Scroller::HandleScrollDown});
}

void Scroller::HandleScrollUp()
{
    if(isHovered_ == false)
        return;

    firstElementIndex++;

    const auto parent = (Element *)parent_;
    if(firstElementIndex + listSize > activeChildCount)
    {
        firstElementIndex--;
    }
}

void Scroller::HandleScrollDown()
{
    if(isHovered_ == false)
        return;

    firstElementIndex--;

    if(firstElementIndex < 0)
    {
        firstElementIndex = 0;
    }
}
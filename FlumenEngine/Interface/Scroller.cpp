#include "Scroller.h"
#include "FlumenEngine/Core/InputHandler.hpp"

void Scroller::Setup(Element *parent, int maximumVisibleCount, int activeChildCount)
{
    this->maximumVisibleCount = maximumVisibleCount;

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

    if(maximumVisibleCount < activeChildCount)
    {
        auto &children = parent->GetChildren();

        for(int i = 0; i < activeChildCount; ++i)
        {
            auto child = *children.Get(i);

            if(i >= firstElementIndex && i < firstElementIndex + maximumVisibleCount)
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

void Scroller::HandleScrollDown()
{
    if(isHovered_ == false)
        return;

    firstElementIndex++;

    const auto parent = (Element *)parent_;
    if(firstElementIndex + maximumVisibleCount > activeChildCount)
    {
        firstElementIndex--;
    }
}

void Scroller::HandleScrollUp()
{
    if(isHovered_ == false)
        return;

    firstElementIndex--;

    if(firstElementIndex < 0)
    {
        firstElementIndex = 0;
    }
}
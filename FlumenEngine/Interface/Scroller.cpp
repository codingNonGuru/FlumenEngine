#include "Scroller.h"
#include "FlumenEngine/Core/InputHandler.hpp"
#include "FlumenEngine/Interface/Interface.hpp"

void Scroller::HandleInitialize()
{
    MakeScrollable();

    maximumVisibleCount = 0;
}

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

void Scroller::HandleLateUpdate()
{
    const auto parent = (Element *)parent_;

    if(maximumVisibleCount == 0)
        return;

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
}

void Scroller::HandleExtremelyLateUpdate()
{
    const auto parent = (Element *)parent_;

    SetWidth(parent->GetSize().x);

    SetHeight(parent->GetSize().y);
}

void Scroller::HandleEnable()
{
    const auto parent = (Element *)parent_;
    
    SetPosition(parent->GetPosition());

    SetInteractivity(true);

    MakeTransparentToClicks();
}

void Scroller::HandleDisable()
{
    SetInteractivity(false);
}

void Scroller::HandleScrollDown()
{
    if(isHovered_ == false)
        return;

    firstElementIndex++;

    //const auto parent = (Element *)parent_;
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
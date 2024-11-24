#include "LayoutGroup.h"
#include "FlumenEngine/Interface/ElementFactory.h"
#include "FlumenEngine/Interface/Scroller.h"

void LayoutGroup::MakeScrollable(int listSize, int activeChildCount)
{
    if(scroller != nullptr)
        return;

    scroller = ElementFactory::BuildElement <Scroller> 
    (
        {drawOrder_ + 1, {nullptr}}
    );

    scroller->Setup(this, listSize, activeChildCount);
    scroller->Enable();
}

void LayoutGroup::SetScrollableChildCount(int childCount)
{
    scroller->SetActiveChildCount(childCount);
}
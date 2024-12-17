#include "SimpleList.h"
#include "FlumenEngine/Interface/ElementFactory.h"
#include "FlumenEngine/Interface/Scroller.h"

void SimpleList::MakeScrollable(int maximumVisibleCount, int maximumActiveCount)
{
    if(scroller != nullptr)
        return;

    scroller = ElementFactory::BuildElement <Scroller> 
    (
        {drawOrder_ + 1, {nullptr}}
    );

    scroller->Setup(this, maximumVisibleCount, maximumActiveCount);
    scroller->Enable();

    SetFixedElementCount(maximumVisibleCount);
}

void SimpleList::SetScrollableChildCount(int maximumActiveCount)
{
    scroller->SetActiveChildCount(maximumActiveCount);
}
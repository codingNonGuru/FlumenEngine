#pragma once

#include "FlumenCore/Delegate/Delegate.hpp"

#include "FlumenEngine/Interface/Element.hpp"

class Scroller : public Element
{
    Delegate onScrollUp;

    Delegate onScrollDown;

    int firstElementIndex;

    int maximumVisibleCount;

    int activeChildCount;

    void HandleUpdate() override;

    void HandleEnable() override;

    void HandleDisable() override;

    void HandleScrollUp();

    void HandleScrollDown();

public:
   void Setup(Element *, int, int);

   void SetActiveChildCount(int);
};